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
} t_segmento;

typedef struct {
    uint32_t direccionBase; 
    uint32_t tamanioHueco; 
} t_hueco_libre;

typedef struct {
    uint32_t pid; 
    t_list* tablaDeSegmentosAsociada;
} t_proceso; 

typedef struct {
    uint32_t pid; 
    t_segmento* segmento; 
} t_peticion; 

extern t_segmento* segmento0; 
extern void* espacioDeUsuario; 
extern t_list* huecosLibres; 
void liberarEspacioDeUsuario ();
void liberarSegmentoCompartido ();

void creacionEspacio(); 
void creacionListaHuecosLibres(); 

t_segmento* crearSegmentoCompartido(); 
void ubicarSegmentosEnEspaciosLibres(t_peticion* peticion); 
void ubicarSegmentosPorFirst(t_peticion* peticion); 

#endif 