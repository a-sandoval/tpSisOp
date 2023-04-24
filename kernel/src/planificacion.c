#include "kernel/include/planificacion.h"

int32_t procesosCreados = 0; 
t_list* pcbs_new; 
t_list* pcbs_ready;
t_list* pcbs_block;
t_list* pcb_instrucciones; 

void inicializarListasPCBS(){
    pcbs_new = list_create();
    pcbs_ready = list_create();
    pcbs_block = list_create();
    pcb_instrucciones = list_create();
}

//Destruir listas PCBS
void destruirListaPCB(t_list* pcb){
    list_destroy_and_destroy_elements(pcb,(void*)destruirPCB);
}

t_pcb* crearPCB() {

    t_pcb* nuevoPCB = malloc(sizeof(t_pcb)); 
    nuevoPCB->estado = NEW; 
    nuevoPCB->pid = procesosCreados; 
    nuevoPCB->instrucciones = pcb_instrucciones; 
    nuevoPCB->estimadoProximaRafaga = obtenerEstimacionInicial(); 
    nuevoPCB->llegadaAReady = temporal_create(); 

    procesosCreados++; //para el nuevo pid

    log_info(logger,"Se crea el proceso <%d> en NEW", nuevoPCB->pid); 

    return nuevoPCB; 

}
 

void destruirPCB(t_pcb* pcb){
    free(pcb);
}

int obtenerEstimacionInicial() {

    return config_get_int_value(config,"ESTIMACION_INICIAL"); 
    
}

 t_pcb* proximoAEjecutarFIFO(){
    return (t_pcb*)list_get(pcbs_ready, 0);
}

//void cambiarDeNewAReady(); CAMBIAR DE ESTADO

