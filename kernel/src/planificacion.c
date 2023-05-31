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


int *instanciasRecursos;
t_list *recursos;
char **nombresRecursos;
int cantidadRecursos;


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
    int tamanio = tamanioArrayCharDoble(nombresRecursos);

    for (int i = 0; i < tamanio; i++)
    {
        if (!strcmp(recurso, nombresRecursos[i]))
        {
            return i;
        }
    }

    return -1;
}


//Funciones auxiliares de colas de bloqueo para iterar e imprimir

//Calcula el tamanio de un array char doble que vienen de los config y termina en NULL
int tamanioArrayCharDoble(char**arreglo){

	    int tamano = 0;

	    while (arreglo[tamano] != NULL) {
	        tamano++;
	    }

	    return tamano;

}

void imprimirVectorEnteros(int* vector, int tamanio){

	for(int i=0;i<tamanio;i++){
		printf("%d \t",vector[i]);
	}

}


void closurePCB(void* pcbActual){

	t_pcb* pcb = (t_pcb*) pcbActual;

	printf("Estado del PCB: %s \t",estadosProcesos[pcb->estado]);

	printf("PID del PCB: %d",pcb->pid);

	putchar('\n');

}


void closureMatriz(void* colaBloqueados){

	t_list* cola = (t_list*)colaBloqueados;

	list_iterate(cola, closurePCB);

	putchar('\n');

}

void imprimirMatrizRecursosColaBloqueados(t_list* matriz,int tamanio){

    list_iterate(matriz, closureMatriz);

    putchar('\n');

}

void imprimirArrayStrings(char** array,int tamanio){

	for(int i =0;i<tamanio;i++){
		printf("Elemento %d: %s \t ",i,array[i]);
	}
	putchar('\n');
}




//Destruccion de colas de bloqueo
void destruirInstanciasRecursos(){
	free(instanciasRecursos);
}

void destruirArrayCharDoble(char**array){

	int tamanio=tamanioArrayCharDoble(array);

	for(int i=0;i<tamanio;i++){
		free(array[i]);
	}
	free(array);
}

void colaBloqueadosDestroyer(void* colaBloqueados){
	list_destroy_and_destroy_elements(colaBloqueados,(void*)destruirPCB);
}

void destruirRecursos(){
	list_destroy_and_destroy_elements(recursos, colaBloqueadosDestroyer);
}

void liberarColasBloqueo(){
	destruirInstanciasRecursos();
	destruirArrayCharDoble(nombresRecursos);
	destruirRecursos();
}

// crear colas de bloqueo
void crearColasBloqueo()
{
	instanciasRecursos=NULL;

	recursos = list_create();

    nombresRecursos = obtenerRecursos();

    char **instancias = obtenerInstanciasRecursos();

    int tamanio = tamanioArrayCharDoble(instancias);

    cantidadRecursos=tamanio;

    for (int i = 0; i < tamanio; i++)
    {

        int instanciasConvertEntero = atoi(instancias[i]);

        instanciasRecursos=realloc(instanciasRecursos,(i+1)*sizeof(int));

        instanciasRecursos[i]=instanciasConvertEntero;

        /*
        imprimirVectorEnteros(instanciasRecursos,i+1);
        putchar('\n');
        */


        t_list *colaBloqueo = list_create();

        list_add(recursos, (void *)colaBloqueo);
    }

    destruirArrayCharDoble(instancias);
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
