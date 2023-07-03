#ifndef MEMORIA_COMPACTACION_H
#define MEMORIA_COMPACTACION_H

#include <commons/collections/list.h>
#include <commons/config.h>
#include "auxiliaresMemoria.h"
#include "manejoSegmentacion.h"

extern void* espacioDeUsuario; 
extern t_list* huecosLibres; 
extern t_segmento* segmento0; 
extern t_list* tablaDeTablasDeSegmentos; 

extern t_config* config; 

#endif