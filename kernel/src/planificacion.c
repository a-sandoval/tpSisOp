#include "kernel/include/planificacion.h"

sem_t hayProcesosReady;
sem_t hayProcesosNuevos;
t_list* pcbsNEW; 
t_list* pcbsREADY;
int32_t procesosCreados = 0; 
pthread_mutex_t mutexListaNew; 
sem_t semGradoMultiprogramacion;

int gradoMultiprogramacion; 

char* estadosProcesos[5] = {"NEW","READY","EXEC","BLOCK","SALIDA"};

// Planificacion en si

void loggearCambioDeEstado(uint32_t pid, estadoProceso anterior, estadoProceso actual) {

    log_info(logger, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pid , estadosProcesos[anterior], estadosProcesos[actual]);

}

void planificarALargoPlazo(){

    while (1) {
        sem_wait(&hayProcesosNuevos); 
        
        sem_wait(&semGradoMultiprogramacion);

        t_pcb* pcb = obtenerSiguienteAReady();
        estadoProceso estadoAnterior= pcb->estado;
        pcb->estado=READY;

        loggearCambioDeEstado(pcb->pid, estadoAnterior,pcb->estado); 

        //list_iterate(pcb->instrucciones, instruct_print);
        encolar(pcbsREADY, pcb);
        
        sem_post(&hayProcesosReady);
        sem_post(&semGradoMultiprogramacion);
    }
}

void planificarACortoPlazo(t_pcb* (*proximoAEjecutar)()) {

    while (1) {
        sem_wait(&hayProcesosReady); 
        t_pcb* aEjecutar = proximoAEjecutar();

        estadoProceso estadoAnterior= aEjecutar->estado;
        aEjecutar->estado = EXEC; 

        loggearCambioDeEstado(aEjecutar->pid, estadoAnterior,aEjecutar->estado);

        procesarPCB(aEjecutar);
        // ESTO ES ALGO DEL MOMENTO, NO LO HACE EL KERNEL!!!
        sleep (2);
        aEjecutar->estado = SALIDA;
        enviarMensaje("Terminado", aEjecutar->socketPCB);
        
    }
}

void planificarACortoPlazoSegunAlgoritmo(){
    char* algoritmoPlanificador = obtenerAlgoritmoPlanificacion();

    if(!strcmp(algoritmoPlanificador,"FIFO")){
        planificarACortoPlazo(proximoAEjecutarFIFO);
    }
    else if(!strcmp(algoritmoPlanificador,"HRRN")){
        planificarACortoPlazo(proximoAEjecutarHRRN);
    }
    else{
        log_error(loggerError,"Algoritmo invalido. Debe ingresar FIFO o HRRN");
        abort();
    }
}

 t_pcb* proximoAEjecutarFIFO(){
    return desencolar(pcbsREADY);
}

 t_pcb* proximoAEjecutarHRRN(){
    //TODO
    return NULL;
}

// Semaforos

void inicializarSemaforos(){
    pthread_mutex_init(&mutexListaNew,NULL);
    sem_init(&hayProcesosReady,0,0);
    sem_init(&hayProcesosNuevos,0,0);
    gradoMultiprogramacion=obtenerGradoMultiprogramacion(); 
    sem_init(&semGradoMultiprogramacion,0,gradoMultiprogramacion);
}

// Operaciones con t_pcb

t_pcb* crearPCB() {

    t_pcb* nuevoPCB = malloc(sizeof(t_pcb)); 
    nuevoPCB->estado = NEW; 
    nuevoPCB->pid = procesosCreados; 
    nuevoPCB->programCounter = 0;
    nuevoPCB->instrucciones = list_create(); 
    nuevoPCB->estimadoProximaRafaga = obtenerEstimacionInicial(); 
    nuevoPCB->llegadaAReady = temporal_create(); 
    nuevoPCB->tablaDeArchivos = list_create(); 
    nuevoPCB->tablaDeSegmentos=list_create(); 
    nuevoPCB->registrosCPU = crearDiccionarioDeRegistros(); 

    procesosCreados++; //para el nuevo pid

    return nuevoPCB; 

}

void destruirPCB(t_pcb* pcb){
    list_destroy_and_destroy_elements(pcb->instrucciones,(void*)destruirInstruccion); 
    dictionary_destroy_and_destroy_elements(pcb->registrosCPU, (void*)destruirRegistro); 
    free(pcb);
}

t_dictionary* crearDiccionarioDeRegistros() {

    t_dictionary* registros = dictionary_create(); 

    dictionary_put(registros, "AX",  "0000"); 
    dictionary_put(registros, "BX",  "0000"); 
    dictionary_put(registros, "CX",  "0000"); 
    dictionary_put(registros, "DX",  "0000"); 
    dictionary_put(registros, "EAX", "00000000"); 
    dictionary_put(registros, "EBX", "00000000");
    dictionary_put(registros, "ECX", "00000000");
    dictionary_put(registros, "EDX", "00000000"); 
    dictionary_put(registros, "RAX", "0000000000000000"); 
    dictionary_put(registros, "RBX", "0000000000000000");
    dictionary_put(registros, "RCX", "0000000000000000");
    dictionary_put(registros, "RDX", "0000000000000000"); 

    return registros;

}

// Lista de PCBs

void inicializarListasPCBS(){
    pcbsNEW = list_create();
    pcbsREADY = list_create();
}

void destruirListaPCB(t_list* pcbs){
    list_destroy_and_destroy_elements(pcbs,(void*)destruirPCB);
}

void ingresarANew(t_pcb *pcb) {
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

void encolar(t_list* pcbs,t_pcb* pcb){
    list_add(pcbs,(void *)pcb);
}

t_pcb* desencolar(t_list* pcbs){
    return (t_pcb*)list_remove(pcbs, 0);
}

// frees

void destruirInstruccion(t_instruccion* instruccion) {
    free(instruccion); 
}

void destruirRegistro(char* registro) {
    free(registro); 
}

// Configuracion

int obtenerEstimacionInicial() {
    return config_get_int_value(config,"ESTIMACION_INICIAL"); 
}

int obtenerGradoMultiprogramacion(){
    return config_get_int_value(config,"GRADO_MAX_MULTIPROGRAMACION");
}

char* obtenerAlgoritmoPlanificacion(){
    return confGet("ALGORITMO_PLANIFICACION");
}


void instruct_print(void *value) {
    log_info(logger, "Que linda mi instruccion: %s", (char *)value);
}

