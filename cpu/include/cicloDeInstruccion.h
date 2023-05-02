#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/global.h"
#include <commons/collections/list.h>
#include "shared/include/configuraciones.h"

typedef enum estadoProceso{
    NEW, 
    READY,
    EXEC,
    BLOCK,
    SALIDA
} estadoProceso; 

typedef struct {
    char AX[4], BX[4], CX[4], DX[4];
    char EAX[8], EBX[8], ECX[8], EDX[8];
    char RAX[16], RBX[16], RCX[16], RDX[16];
}t_reg;

typedef struct { 
//ver tipos de datos para hacer el sizeof mas legible
    uint32_t pid; 
    uint32_t instruccionesLength;
    t_list* instrucciones; 
    estadoProceso estado; 
    int programCounter;   
    t_reg* registrosCPU;
    t_list* tablaDeSegmentos; 
    t_list* tablaDeArchivos; 
} t_contexto; 

void cicloDeInstruccion();
void fetch();
void decode();
void execute();

void set_c(char* , char* );
int obtenerTiempoEspera();


#endif 