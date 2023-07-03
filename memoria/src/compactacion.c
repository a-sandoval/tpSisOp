#include "memoria/include/compactacion.h"

t_list* todosLosSegmentos; 

void compactar(){
    
    log_info(logger,"Solicitud de Compactación"); 
    usleep(config_get_int_value(config,"RETARDO_COMPACTACION") * 1000); 
    reubicacionHuecosLibres();
    asignarProcesoalHueco();
    actualizarTablas();
    // log minimo:
    // Resultado Compactación: Por cada segmento de cada proceso se deberá imprimir una línea con el siguiente formato:
    // “PID: <PID> - Segmento: <ID SEGMENTO> - Base: <BASE> - Tamaño <TAMAÑO>”

}

void reubicacionHuecosLibres() {

    todosLosSegmentos = list_create(); 

    unificarSegmentos(); 

    t_segmento* segmento; 

    uint32_t direccionActual = segmento0->tamanio; 

    for (int i = 0; i < list_size(todosLosSegmentos); i++) {
        
        //Aca iria tambien la parte de escribir donde corresponda las cosas.
        segmento = list_get(todosLosSegmentos, i);
        segmento->direccionBase = direccionActual;
        direccionActual += segmento->tamanio;
    }

}

   

void unificarSegmentos() {
    
    t_proceso* temp1; 
    t_segmento* temp2; 

    for (int i=0;i<list_size(tablaDeTablasDeSegmentos);i++){

        temp1 = (t_proceso*)list_get(tablaDeTablasDeSegmentos,i); 

        for(int j=0; i<list_size(temp1->tablaDeSegmentosAsociada);j++) {
            
            temp2 = (t_segmento*)list_get(temp1->tablaDeSegmentosAsociada,j); 
            list_add_sorted(todosLosSegmentos, (void*)temp2,direccionMasBaja); 
            
        }   
    }

}



bool direccionBaseMenor (void * unSegmento, void * otroSegmento) {
    t_segmento *segmentoUno = (t_segmento *) unSegmento;
	t_segmento *segmentoDos = (t_segmento *) otroSegmento;
	return (segmentoUno->direccionBase < segmentoDos->direccionBase); 
}
	
void asignarProcesoalHueco(){
    // literalmente cargar el segmento al hueco
}

void actualizarTablas(){
    //actualizar tabla de huecos libres
    //actuaizar dirrecciones, espacio de usuario y todo el bardo
}
 