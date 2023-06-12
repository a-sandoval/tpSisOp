#ifndef MEMORIA_CONEXION_KERNEL_H
#define MEMORIA_CONEXION_KERNEL_H

#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include  <commons/log.h>
#include<commons/config.h>
#include <commons/collections/list.h>
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "manejoSegmentacion.h"

extern t_config* config;
extern t_log* logger; 
extern t_segmento* segmento0; 
extern void* espacioDeUsuario; 
extern t_list* huecosLibres; 
extern int sockets[3];

int ejecutarServidorKernel(int*);
t_list* crearTablaDeSegmentosInicial(); 
void enviarTablaSegmentos(t_list* tablaDeSegmentos);
void agregarSegmentoAPaquete(t_paquete* paquete, t_segmento* segmento);


#endif