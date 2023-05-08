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

    uint32_t pid; 
    estadoProceso estado; 
    int programCounter; 
    uint32_t registrosSize;  
    t_dictionary* registrosCPU;  
    uint32_t instruccionesLength;
    t_list* instrucciones; 
    uint32_t tablaDeSegmentosSize;
    t_list* tablaDeSegmentos; 
    uint32_t tablaDeArchivosSize;
    t_list* tablaDeArchivos; 

} t_contexto; 

char* instruccionAEjecutar; 
char** elementosInstruccion; 
int instruccionActual; 
int cantParametros;



void cicloDeInstruccion();
void fetch();
void decode();
void execute();
int buscar(char *elemento, char **lista); 


void set_c(char* , char* );
void yield_c(); 
void exit_c(); 

int obtenerTiempoEspera();

// Las ioguientes funciones están encanutadas de conexionesCPU, lit lo mismo

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION
void* serializar_contextoEjecucion(t_paquete* paquete, int bytes);
void enviar_contexto();
void enviarContextoActualizado(); 

#endif 