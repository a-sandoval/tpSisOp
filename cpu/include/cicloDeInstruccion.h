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
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include<commons/temporal.h>

t_contexto *contextoActual;

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
    uint32_t pid; 
    uint32_t instruccionesLength;
    t_list* instrucciones; 
    estadoProceso estado; 
    int programCounter;   
    t_dictionary* registrosCPU;
    t_list* tablaDeSegmentos; 
    t_list* tablaDeArchivos; 
} t_contexto; 

void cicloDeInstruccion(t_contexto* contextoActual);
void fetch();
void decode();
void execute();

void set_c(char* , char* );
int obtenerTiempoEspera();

// Las ioguientes funciones están encanutadas de conexionesCPU, lit lo mismo

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION
void* serializar_contextoEjecucion(t_paquete* paquete, int bytes){
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void enviar_contexto(){ 
    t_paquete* paquete = malloc(sizeof(t_paquete));
    t_contexto* contextoEjecucion;
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
	paquete->buffer = malloc(sizeof(t_buffer));
   
    // cargo todos los valores en el paquete
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregarAPaquete(paquete,&contextoEjecucion->registrosCPU, sizeof(contextoEjecucion->registrosCPU)); // a chequear ese ampersand
    agregarAPaquete(paquete,(void *)&contextoEjecucion->instruccionesLength, sizeof(contextoEjecucion->instruccionesLength));
    agregarAPaquete(paquete,contextoEjecucion->instrucciones, contextoEjecucion->instruccionesLength);
    agregarAPaquete(paquete,(void *)contextoEjecucion->estado, sizeof(estadoProceso));

    enviarPaquete(paquete,socketCliente);

	eliminarPaquete(paquete);
}

#endif 