#include "kernel/include/planificacion.h"

sem_t hayProcesosReady;
sem_t hayProcesosNuevos;
t_list *pcbsNEW;
t_list *pcbsREADY;
int32_t procesosCreados = 0;
pthread_mutex_t mutexListaNew;
sem_t semGradoMultiprogramacion;
char *pidsInvolucrados;
int64_t rafagaCPU;

t_list *instanciasRecursos;
t_list *recursos;
char **nombresRecursos;

int gradoMultiprogramacion;

char *estadosProcesos[5] = {"NEW", "READY", "EXEC", "BLOCK", "SALIDA"};

// Planificacion en si

void loggearCambioDeEstado(uint32_t pid, estadoProceso anterior, estadoProceso actual)
{

    log_info(logger, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pid, estadosProcesos[anterior], estadosProcesos[actual]);
}

void planificarALargoPlazo()
{
    while (1)
    {
        sem_wait(&hayProcesosNuevos);

        sem_wait(&semGradoMultiprogramacion);

        t_pcb *pcb = obtenerSiguienteAReady();
        pcb->tiempoEnReady = temporal_create();
        estadoProceso estadoAnterior = pcb->estado;
        pcb->estado = READY;

        loggearCambioDeEstado(pcb->pid, estadoAnterior, pcb->estado);

        encolar(pcbsREADY, pcb);

        pidsInvolucrados = string_new();
        listarPIDS(pcbsREADY);
        log_info(logger, "Cola Ready <%s>: [%s]", obtenerAlgoritmoPlanificacion(), pidsInvolucrados);
        free(pidsInvolucrados);

        sem_post(&hayProcesosReady);
    }
}

void agregarPID(void *value)
{
    t_pcb *pcb = (t_pcb *)value;
    char *pid = string_itoa(pcb->pid);
    string_append_with_format(&pidsInvolucrados, " %s ", pid);
}

void listarPIDS(t_list *pcbs)
{
    list_iterate(pcbs, agregarPID);
}

void imprimirRegistros(t_dictionary *registros)
{
    log_info(logger, "AX: %s", (char *)dictionary_get(registros, "AX"));
}

void planificarACortoPlazo(t_pcb *(*proximoAEjecutar)())
{

    crearColasBloqueo(recursos, instanciasRecursos);

    while (1)
    {
        sem_wait(&hayProcesosReady);
        t_pcb *aEjecutar = proximoAEjecutar();
        t_contexto *contextoEjecucion = malloc(sizeof(t_contexto));
        iniciarContexto(contextoEjecucion);

        detenerYDestruirCronometro(aEjecutar->tiempoEnReady);
        estadoProceso estadoAnterior = aEjecutar->estado;
        aEjecutar->estado = EXEC;

        loggearCambioDeEstado(aEjecutar->pid, estadoAnterior, aEjecutar->estado);

        contextoEjecucion = procesarPCB(aEjecutar);
        
        switch(contextoEjecucion->motivoDesalojo->comando)
        {
        case IO:
            io_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case WAIT:
            wait_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case SIGNAL:
            signal_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_OPEN:
            fopen_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_CLOSE:
            fclose_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_SEEK:
            fseek_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_READ:
            fread_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_WRITE:
            fwrite_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_TRUNCATE:
            ftruncate_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case CREATE_SEGMENT:
            createSegment_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case DELETE_SEGMENT:
            deleteSegment_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case YIELD:
            yield_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case EXIT:
            exit_s(aEjecutar, contextoEjecucion->motivoDesalojo->parametros);
            break;
        default:
            enviarMensaje("Terminado", aEjecutar->socketPCB);
            destruirPCB(aEjecutar);
            break;
        }
    }
}





// Devuelve el indice que se corresponde al recurso correspondiente, -1 si no lo encuentra
int indiceRecurso(char *recurso)
{
    int tamanio = length(nombresRecursos);

    for (int i = 0; i < tamanio; i++)
    {
        if (!strcmp(recurso, nombresRecursos[i]))
        {
            return i;
        }
    }

    return -1;
}

// crear colas de bloqueo
void crearColasBloqueo(t_list *recursosUso, t_list *instanciasUso)
{

    nombresRecursos = obtenerRecursos();

    char **instancias = obtenerInstanciasRecursos();

    int tamanio = length(instancias);

    for (int i = 0; i < tamanio; i++)
    {

        int cantInstanConvert = atoi(instancias[i]);

        list_add(instanciasUso, (void *)&cantInstanConvert);

        t_list *colaBloqueo = list_create();

        list_add(recursosUso, (void *)colaBloqueo);
    }
}

// caso bloqueo es por I/O
void bloqueoIO(t_pcb *pcb, int tiempo)
{

    pthread_t pcb_bloqueado;

    if (!pthread_create(&pcb_bloqueado, NULL, (void *)bloquearIO, (void *)&tiempo))
    {
        pthread_detach(pcb_bloqueado);
    }
    else
    {
        log_error(loggerError, "Error en la creacion de hilo para realizar I/O, Abort");
        abort();
    }
}

void bloquearIO(int tiempo)
{
    sleep(tiempo); // sleep por la cantidad indicada en el motivo
}

void detenerYDestruirCronometro(t_temporal *cronometroReady)
{

    temporal_stop(cronometroReady);
    temporal_destroy(cronometroReady);
}

void planificarACortoPlazoSegunAlgoritmo()
{
    char *algoritmoPlanificador = obtenerAlgoritmoPlanificacion();

    if (!strcmp(algoritmoPlanificador, "FIFO"))
    {
        planificarACortoPlazo(proximoAEjecutarFIFO);
    }
    else if (!strcmp(algoritmoPlanificador, "HRRN"))
    {
        planificarACortoPlazo(proximoAEjecutarHRRN);
    }
    else
    {
        log_error(loggerError, "Algoritmo invalido. Debe ingresar FIFO o HRRN");
        abort();
    }
}

t_pcb *proximoAEjecutarFIFO()
{
    return desencolar(pcbsREADY);
}

void *mayorRR(void *unPCB, void *otroPCB)
{

    return (calcularRR(unPCB) >= calcularRR(otroPCB)) ? unPCB : otroPCB;
}

void estimacionNuevaRafaga(t_pcb *pcb)
{

    if (strcmp(obtenerAlgoritmoPlanificacion(), "HRRN"))
    {
        calcularEstimadoProximaRafaga(pcb, rafagaCPU);
    }
}

void calcularEstimadoProximaRafaga(t_pcb *pcb, int64_t rafagaReal)
{

    double alfa = obtenerAlfaEstimacion();

    double estimadoRafaga = alfa * rafagaReal + (1 - alfa) * pcb->estimadoProximaRafaga;

    pcb->estimadoProximaRafaga = estimadoRafaga;
}

double calcularRR(void *elem)
{

    t_pcb *pcb = (t_pcb *)elem;

    temporal_stop(pcb->tiempoEnReady);

    double waitTime = temporal_gettime(pcb->tiempoEnReady);

    temporal_resume(pcb->tiempoEnReady);

    double estimatedServiceTime = pcb->estimadoProximaRafaga;

    return (waitTime + estimatedServiceTime) / estimatedServiceTime;
}

t_pcb *proximoAEjecutarHRRN()
{

    return list_get_maximum(pcbsREADY, mayorRR);
}

// Semaforos

void inicializarSemaforos()
{
    pthread_mutex_init(&mutexListaNew, NULL);
    sem_init(&hayProcesosReady, 0, 0);
    sem_init(&hayProcesosNuevos, 0, 0);
    gradoMultiprogramacion = obtenerGradoMultiprogramacion();
    sem_init(&semGradoMultiprogramacion, 0, gradoMultiprogramacion);
}

// Operaciones con t_pcb

t_pcb *crearPCB()
{

    t_pcb *nuevoPCB = malloc(sizeof(t_pcb));
    nuevoPCB->estado = NEW;
    nuevoPCB->pid = procesosCreados;
    nuevoPCB->programCounter = 0;
    nuevoPCB->instrucciones = list_create();
    nuevoPCB->estimadoProximaRafaga = obtenerEstimacionInicial();
    nuevoPCB->tablaDeArchivos = list_create();
    nuevoPCB->tablaDeSegmentos = list_create();
    nuevoPCB->registrosCPU = crearDiccionarioDeRegistros();

    procesosCreados++; // para el nuevo pid

    return nuevoPCB;
}

void destruirPCB(t_pcb *pcb)
{
    list_destroy_and_destroy_elements(pcb->instrucciones, (void *)destruirInstruccion);
    list_destroy_and_destroy_elements(pcb->tablaDeArchivos, (void *)destruirInstruccion);
    list_destroy_and_destroy_elements(pcb->tablaDeSegmentos, (void *)destruirInstruccion);
    dictionary_destroy_and_destroy_elements(pcb->registrosCPU, (void *)destruirRegistro);
    free(pcb);
}

t_dictionary *crearDiccionarioDeRegistros()
{

    t_dictionary *registros = dictionary_create();

    char *AX = malloc(sizeof(char) * (4 + 1));
    char *BX = malloc(sizeof(char) * (4 + 1));
    char *CX = malloc(sizeof(char) * (4 + 1));
    char *DX = malloc(sizeof(char) * (4 + 1));
    char *EAX = malloc(sizeof(char) * (8 + 1));
    char *EBX = malloc(sizeof(char) * (8 + 1));
    char *ECX = malloc(sizeof(char) * (8 + 1));
    char *EDX = malloc(sizeof(char) * (8 + 1));
    char *RAX = malloc(sizeof(char) * (16 + 1));
    char *RBX = malloc(sizeof(char) * (16 + 1));
    char *RCX = malloc(sizeof(char) * (16 + 1));
    char *RDX = malloc(sizeof(char) * (16 + 1));

    strncpy(AX, "0000\0", 5);
    strncpy(BX, "0000\0", 5);
    strncpy(CX, "0000\0", 5);
    strncpy(DX, "0000\0", 5);
    strncpy(EAX, "00000000\0", 9);
    strncpy(EBX, "00000000\0", 9);
    strncpy(ECX, "00000000\0", 9);
    strncpy(EDX, "00000000\0", 9);
    strncpy(RAX, "0000000000000000\0", 17);
    strncpy(RBX, "0000000000000000\0", 17);
    strncpy(RCX, "0000000000000000\0", 17);
    strncpy(RDX, "0000000000000000\0", 17);
    dictionary_put(registros, "AX", AX);
    dictionary_put(registros, "BX", BX);
    dictionary_put(registros, "CX", CX);
    dictionary_put(registros, "DX", DX);
    dictionary_put(registros, "EAX", EAX);
    dictionary_put(registros, "EBX", EBX);
    dictionary_put(registros, "ECX", ECX);
    dictionary_put(registros, "EDX", EDX);
    dictionary_put(registros, "RAX", RAX);
    dictionary_put(registros, "RBX", RBX);
    dictionary_put(registros, "RCX", RCX);
    dictionary_put(registros, "RDX", RDX);

    return registros;
}

// Lista de PCBs

void inicializarListasPCBS()
{
    pcbsNEW = list_create();
    pcbsREADY = list_create();
}

void destruirListaPCB(t_list *pcbs)
{
    list_destroy_and_destroy_elements(pcbs, (void *)destruirPCB);
}

void ingresarANew(t_pcb *pcb)
{
    pthread_mutex_lock(&mutexListaNew);
    encolar(pcbsNEW, pcb);
    log_info(logger, "Se crea el proceso <%d> en NEW", pcb->pid);
    pthread_mutex_unlock(&mutexListaNew);
}

t_pcb *obtenerSiguienteAReady()
{
    pthread_mutex_lock(&mutexListaNew);
    t_pcb *pcb = desencolar(pcbsNEW);
    pthread_mutex_unlock(&mutexListaNew);
    return pcb;
}

void encolar(t_list *pcbs, t_pcb *pcb)
{
    list_add(pcbs, (void *)pcb);
}

t_pcb *desencolar(t_list *pcbs)
{
    return (t_pcb *)list_remove(pcbs, 0);
}

// frees

void destruirInstruccion(t_instruccion *instruccion)
{
    free(instruccion);
}

void destruirRegistro(char *registro)
{
    free(registro);
}

// Configuracion

double obtenerEstimacionInicial()
{
    return config_get_double_value(config, "ESTIMACION_INICIAL");
}

int obtenerGradoMultiprogramacion()
{
    return config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
}

char *obtenerAlgoritmoPlanificacion()
{
    return confGet("ALGORITMO_PLANIFICACION");
}

double obtenerAlfaEstimacion()
{
    return config_get_double_value(config, "HRRN_ALFA");
}

char **obtenerRecursos() {

    return config_get_array_value(config, "RECURSOS");
}

char **obtenerInstanciasRecursos(){
    return config_get_array_value(config, "INSTANCIAS_RECURSOS");
}

void instruct_print(void *value)
{
    log_info(logger, "Que linda mi instruccion: %s", (char *)value);
}
