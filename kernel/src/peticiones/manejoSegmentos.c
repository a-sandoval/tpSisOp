#include "kernel/include/peticiones/manejoSegmentos.h"

void recibirTablaDeSegmentosActualizada(t_pcb* pcb){

	int size, desplazamiento = 0;
	void * buffer;

	buffer = recibirBuffer(conexionAMemoria, &size);

    desplazamiento += sizeof(int);
    //salteo el pid aca porque ya lo tengo del pcb
    desplazamiento += sizeof(uint32_t) + sizeof(int);
    
    t_segmento* segmento;

    list_clean_and_destroy_elements (pcb->tablaDeSegmentos, free);
    
    uint32_t tablaDeSegmentosSize;
    // Desplazamiento: tamaño de la lista de segmentos.
    memcpy(&(tablaDeSegmentosSize), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    
    for (uint32_t i = 0; i < tablaDeSegmentosSize; i++) {

        // Desplazamiento: Tamaño del segmento.

        segmento = deserializarSegmento(buffer, &desplazamiento);
        list_add (pcb->tablaDeSegmentos, segmento);
    }
    free (buffer);
    //log_debug (logger, "Me llegaron %d segmentos, deberian llegar %d.", list_size (pcb->tablaDeSegmentos), tablaDeSegmentosSize);
    desplazamiento += sizeof(int);

}