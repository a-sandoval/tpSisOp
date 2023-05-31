#include "memoria/include/conexionKernel.h"

t_list* crearTablaDeSegmentosInicial() {

    t_segmento *segmento0 = malloc(sizeof(t_segmento)); 
    segmento0->id=0; 
    segmento0->direccionBase=0; 
    segmento0->tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 

    t_list* tablaDeSegmentos = list_create(); 
    list_add(tablaDeSegmentos,(void*)segmento0);

    return tablaDeSegmentos;  

}