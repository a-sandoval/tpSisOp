#include "kernel/include/planificacion.h"



void inicializarListasPCBS(){
    pcbsNEW = list_create();
    pcbsREADY = list_create();
    pcbsBLOCK = list_create();    
}

void destruirListaPCB(t_list* pcbs){
    list_destroy_and_destroy_elements(pcbs,(void*)destruirPCB);
}

t_pcb* crearPCB() {

    t_pcb* nuevoPCB = malloc(sizeof(t_pcb)); 
    nuevoPCB->estado = NEW; 
    nuevoPCB->pid = procesosCreados; 
    nuevoPCB->instrucciones = list_create(); 
    nuevoPCB->estimadoProximaRafaga = obtenerEstimacionInicial(); 
    nuevoPCB->llegadaAReady = temporal_create(); 

    procesosCreados++; //para el nuevo pid

    list_add(pcbsNEW, (void*)nuevoPCB); 

    log_info(logger,"Se crea el proceso <%d> en NEW", nuevoPCB->pid); 

    return nuevoPCB; 

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

void cambiarEstadoNewAReady(){
    t_pcb* pcb=desencolar(pcbsNEW);
    pcb->estado = READY;
    encolar(pcbsREADY,pcb);
}

 t_pcb* proximoAEjecutarFIFO(){
    return desencolar(pcbsREADY);
}