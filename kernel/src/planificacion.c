#include "kernel/include/planificacion.h"

sem_t hayProcesosReady;
sem_t hayProcesosNuevos;
t_list* pcbsNEW; 
t_list* pcbsREADY;
int32_t procesosCreados = 0; 
pthread_mutex_t mutexListaNew; 


void inicializarListasPCBS(){
    pcbsNEW = list_create();
    pcbsREADY = list_create();
}

void destruirListaPCB(t_list* pcbs){
    list_destroy_and_destroy_elements(pcbs,(void*)destruirPCB);
}

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

    procesosCreados++; //para el nuevo pid

    return nuevoPCB; 

}

void ingresarANew(t_pcb *pcb) {

    pthread_mutex_lock(&mutexListaNew);

    list_add(pcbsNEW, (void*)pcb); 
    
    log_info(logger,"Se crea el proceso <%d> en NEW", pcb->pid); 

    pthread_mutex_unlock(&mutexListaNew);

}


void destruirPCB(t_pcb* pcb){
    list_destroy_and_destroy_elements(pcb->instrucciones,(void*)destruirInstruccion); 
    free(pcb);
}

void destruirInstruccion(t_instruccion* instruccion) {
    free(instruccion); 
}

int obtenerEstimacionInicial() {

    return config_get_int_value(config,"ESTIMACION_INICIAL"); 
    
}

int obtenerGradoMultiprogramacion(){
    return config_get_int_value(config,"GRADO_MAX_MULTIPROGRAMACION");
}

t_pcb* desencolar(t_list* pcbs){
    return (t_pcb*)list_remove(pcbs, 0);
}

void encolar(t_list* pcbs,t_pcb* pcb){
    list_add(pcbs,(void *)pcb);
}

void planificarALargoPlazo(){

    while (1) {

        char* estadoActual = "READY"; //Esto es una ilegalidad ?-?

        sem_wait(&hayProcesosNuevos); 

        //sem_wait(sem_multiprogra)
       
        t_pcb* pcb = obtenerSiguienteAReady(); 
    	log_info(logger, "PID: %d - Estado Anterior: %d - Estado Actual: %s", pcb->pid, pcb->estado, estadoActual);
        pcb->estado = READY;
        
        //pthread_mutex_unlock(&mutexListaNew);
        sem_post(&hayProcesosReady);

    }
    
   
}


t_pcb *obtenerSiguienteAReady()
{
    	pthread_mutex_lock(&mutexListaNew);
    	t_pcb *pcb = desencolar(pcbsNEW);
    	pthread_mutex_unlock(&mutexListaNew);
    	return pcb;
}


void planificarACortoPlazo() {
    
    sem_wait(&hayProcesosReady); 
    t_pcb* aEjecutar = desencolar(pcbsREADY);
    aEjecutar->estado = EXEC; 
}

 t_pcb* proximoAEjecutarFIFO(){
    return desencolar(pcbsREADY);
}