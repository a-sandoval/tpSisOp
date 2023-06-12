#ifndef MEMORIA_CONEXION_KERNEL_H
#define MEMORIA_CONEXION_KERNEL_H

#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>

#include<commons/log.h>
#include<commons/config.h>
#include <commons/collections/list.h>
#include "shared/include/utilsServidor.h"
#include "shared/include/utilsCliente.h"
#include "manejoSegmentacion.h"


int ejecutarServidorKernel(int*);

extern t_config* config;
extern t_log* logger; 
extern t_segmento* segmento0; 



#endif