#include "memoria/include/conexionKernel.h"

t_list* crearTablaDeSegmentosInicial() {

    t_segmento segmento0; 
    segmento0.id=0; 
    segmento0.direccionBase=0; 
    segmento0.tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 


}