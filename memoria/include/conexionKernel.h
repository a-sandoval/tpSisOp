#ifndef MEMORIA_CONEXION_KERNEL_H
#define MEMORIA_CONEXION_KERNEL_H
#include<stdio.h>
#include<stdlib.h>

#include<commons/log.h>
#include<commons/config.h>
#include <commons/collections/list.h>

typedef struct {

    uint32_t id; 
    uint32_t direccionBase; 
    uint32_t tamanio; 

}t_segmento; 


extern t_config* config; 
#endif