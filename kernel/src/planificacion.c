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

        retornoContexto(aEjecutar, contextoEjecucion);
        
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

// Semaforos

void inicializarSemaforos()
{
    pthread_mutex_init(&mutexListaNew, NULL);
    sem_init(&hayProcesosReady, 0, 0);
    sem_init(&hayProcesosNuevos, 0, 0);
    gradoMultiprogramacion = obtenerGradoMultiprogramacion();
    sem_init(&semGradoMultiprogramacion, 0, gradoMultiprogramacion);
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


void instruct_print(void *value)
{
    log_info(logger, "Que linda mi instruccion: %s", (char *)value);
}
