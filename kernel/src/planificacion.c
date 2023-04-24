#include "planificacion.h"

int32_t procesosCreados = 0; 
t_list* pcbs_new; 
//t_list* pcbs_ready; 

t_pcb* crearPCB() {

    t_pcb* nuevoPCB = malloc(sizeof(t_pcb)); 
    nuevoPCB->estado = NEW; 
    nuevoPCB->pid = procesosCreados; 
    nuevoPCB->estimadoProximaRafaga = obtenerEstimacionInicial(); 
    nuevoPCB->llegadaAReady = temporal_create(); 

    procesosCreados++; //para el nuevo pid

    log_info(logger,"Se crea el proceso <%d> en NEW", nuevoPCB->pid); 


    return nuevoPCB; 

} 
 
int obtenerEstimacionInicial() {

    return config_get_int_value(config,"ESTIMACION_INICIAL"); 
    
}

 t_pcb proximoAEjecutarFIFO(t_list* procesosReady){
    t_pcb *proximo = (t_pcb*)list_get(procesosReady, 0);
    return *proximo;
}

