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
#include "shared/include/utilsCliente.h"

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
void* espacioDeUsuario; 
void liberarEspacioDeUsuario ();
void liberarSegmentoCompartido ();

t_segmento* crearSegmentoCompartido(); 

#endif 