#include "kernel/include/planificacion.h"

int32_t procesosCreados = 0; 
t_list* pcbs_new; 
t_list* pcbs_ready;
t_list* pcbs_block;


void inicializarListasPCBS(){
    pcbs_new = list_create();
    pcbs_ready = list_create();
    pcbs_block = list_create();    
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

    list_add(pcbs_new,(void*)nuevoPCB); 

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
    return (t_pcb*)list_get(pcbs, 0);    
}

void encolar(t_list* pcbs){
    list_add();
}

void cambiarNewAReady(){

}

 t_pcb* proximoAEjecutarFIFO(){
    return desencolar(pcbs_ready);
}


