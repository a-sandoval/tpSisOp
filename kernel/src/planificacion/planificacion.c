#include "kernel/include/planificacion/planificacion.h"

sem_t hayProcesosReady;
sem_t hayProcesosNuevos;
t_list *pcbsNEW;
t_list *pcbsREADY;
int32_t procesosCreados = 0;
pthread_mutex_t mutexListaNew;
pthread_mutex_t mutexListaReady; 
pthread_mutex_t mutexFS;
sem_t huboPeticion;
sem_t semGradoMultiprogramacion;
int64_t rafagaCPU;

int gradoMultiprogramacion;
char *estadosProcesos[5] = {"NEW", "READY", "EXEC", "BLOCK", "SALIDA"};
int *instanciasRecursos;

// Planificacion en si

void planificarALargoPlazo(){

    while (1)
    {
        sem_wait(&hayProcesosNuevos);

        sem_wait(&semGradoMultiprogramacion);

        t_pcb *pcb = obtenerSiguienteAReady();

        recibirEstructurasInicialesMemoria(pcb); 

        estadoProceso anterior = pcb->estado;
        pcb->estado = READY;
        loggearCambioDeEstado(pcb->pid, anterior, pcb->estado);
        ingresarAReady(pcb); 
        
     
    }
} 


void planificarACortoPlazo(t_pcb *(*proximoAEjecutar)()){

    crearColasBloqueo();

    while (1)
    {
        sem_wait(&hayProcesosReady);
        t_pcb *aEjecutar = proximoAEjecutar();

        detenerYDestruirCronometro(aEjecutar->tiempoEnReady);
        estadoProceso estadoAnterior = aEjecutar->estado;
        aEjecutar->estado = EXEC;

        loggearCambioDeEstado(aEjecutar->pid, estadoAnterior, aEjecutar->estado);

        contextoEjecucion = procesarPCB(aEjecutar);

        rafagaCPU = contextoEjecucion->rafagaCPUEjecutada; 

        retornoContexto(aEjecutar, contextoEjecucion);

    }
}


// Semaforos

void inicializarSemaforos(){   
    gradoMultiprogramacion = obtenerGradoMultiprogramacion();
    pthread_mutex_init(&mutexListaNew, NULL);
    pthread_mutex_init(&mutexListaReady,NULL); 
    pthread_mutex_init(&mutexFS,NULL); 
    sem_init(&hayProcesosNuevos, 0, 0);
    sem_init(&hayProcesosReady, 0, 0);
    sem_init(&semGradoMultiprogramacion, 0, gradoMultiprogramacion);
    sem_init(&huboPeticion, 0, 0);
}

void destruirSemaforos () {
    pthread_mutex_destroy(&mutexListaNew);
    pthread_mutex_destroy(&mutexListaReady);
    pthread_mutex_destroy(&mutexFS);
    sem_close(&hayProcesosNuevos);
    sem_close(&hayProcesosReady);
    sem_close(&semGradoMultiprogramacion);
    sem_close(&huboPeticion);
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

void ingresarAReady(t_pcb *pcb){
    pthread_mutex_lock(&mutexListaReady);
    encolar(pcbsREADY, pcb);
    pcb->tiempoEnReady = temporal_create();
    
    pthread_mutex_unlock(&mutexListaReady);

    sem_post(&hayProcesosReady);

    //Esto es para loggear de la forma pedida 
    pidsInvolucrados = string_new();
    listarPIDS(pcbsREADY);
    log_info(logger, "Cola Ready <%s>: [%s]", obtenerAlgoritmoPlanificacion(), pidsInvolucrados);
    free(pidsInvolucrados);
}

// Logs minimos y obligatorios

void loggearCambioDeEstado(uint32_t pid, estadoProceso anterior, estadoProceso actual){
    log_info(logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estadosProcesos[anterior], estadosProcesos[actual]);
}

