#include "planificacion.h"

t_pcb crearPCB() {

    t_pcb nuevoPCB; 
    nuevoPCB.estado = NEW; 
    nuevoPCB.pid = procesosCreados; 
    nuevoPCB.estimadoProximaRafaga = obtenerEstimacionInicial(); 
    nuevoPCB.llegadaAReady = temporal_create(); 

    procesosCreados++; //para el nuevo pid

    return nuevoPCB; 

} 
 
int obtenerEstimacionInicial() {

    return config_get_int_value(config,"ESTIMACION_INICIAL"); 
    
}

