#ifndef MANEJO_SEGMENTACION_H
#define MANEJO_SEGMENTACION_H

#include<stdio.h>
#include<stdlib.h>

#include<commons/log.h>
#include<commons/config.h>
#include <pthread.h>

#include "shared/include/global.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/configuraciones.h"

typedef struct {
    uint32_t id; 
    uint32_t direccionBase; 
    uint32_t tamanio; 
}t_segmento; 

typedef struct {
    uint32_t direccionBase; 
    uint32_t tamanioHueco; 
} t_hueco_libre; 

extern t_segmento* segmento0; 
extern void* espacioDeUsuario; 
extern t_list* huecosLibres; 
void liberarEspacioDeUsuario ();
void liberarSegmentoCompartido ();

void creacionEspacio(); 
void creacionListaHuecosLibres(); 

t_segmento* crearSegmentoCompartido(); 

#endif 