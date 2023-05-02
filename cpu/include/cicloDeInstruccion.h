#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/global.h"
#include<commons/collections/list.h>
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
    t_list* instrucciones; 
    uint32_t instruccionesLength;
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

void set(char* , char* );
int obtenerTiempoEspera();

char *listaComandos[] = {
    [SET] = "SET",
    [MOV_IN] = "MOV_IN",
    [MOV_OUT] = "MOV_OUT", 
    [IO] = "I/O",
    [F_OPEN] = "F_OPEN",
    [F_CLOSE] = "F_CLOSE", 
    [F_SEEK] = "F_SEEK",
    [F_READ] = "F_READ",
    [F_WRITE] = "F_WRITE", 
    [F_TRUNCATE] = "F_TRUNCATE",
    [WAIT] = "WAIT",
    [SIGNAL] = "SIGNAL",
    [CREATE_SEGMENT] = "CREATE_SEGMENT",
    [DELETE_SEGMENT] = "DELETE_SEGMENT",
    [YIELD] = "YIELD",
    [EXIT] = "EXIT"
};

#endif 