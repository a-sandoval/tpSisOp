#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/temporal.h>
#include <commons/collections/list.h>
#include "shared/include/global.h"
#include "shared/include/configuraciones.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"

extern char *listaComandos[];
extern char* instruccionAEjecutar; 
extern char** elementosInstruccion; 
extern int instruccionActual; 
extern int cantParametros;
//nuevos
extern char* direccionLogica;
extern int tiempo;//miedo
extern char* nombreArchivo;
extern uint32_t posicion;
extern int cantBytes;
extern int tamanio;
extern char* recurso;
extern uint32_t idSegmento;


typedef enum estadoProceso{
    NEW, 
    READY,
    EXEC,
    BLOCK,
    SALIDA
} estadoProceso;  

typedef struct {

        t_comando comando; 
        int parametrosLength;
        char* parametros[3]; 

}t_motivoDeDesalojo; 

typedef struct {

    uint32_t pid; 
    int programCounter;
    uint32_t instruccionesLength;
    t_list* instrucciones; 
    t_dictionary* registrosCPU;  
    uint32_t tablaDeArchivosSize;
    t_list* tablaDeArchivos; 
    uint32_t tablaDeSegmentosSize;
    t_list* tablaDeSegmentos; 
    t_motivoDeDesalojo motivoDesalojo;
    int64_t rafagaCPUEjecutada;

} t_contexto; 

extern t_contexto* contextoEjecucion;

void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU);

void cicloDeInstruccion();
void fetch();
void decode();
void execute();
int buscar(char *elemento, char **lista); 


void set_c(char* , char* );
void yield_c(); 
void exit_c(); 
//Declaración de nuevas funciones
void mov_in(char* , char* );
void mov_out(char* , char* );
void io(int );
void f_open(char* );
void f_close(char* );
void f_seek(char* , uint32_t );
void f_read(char* , char* , int );
void f_write(char* , char* , int );
void f_truncate(char* , int );
void wait_c(char* );
void signal_c(char* );
void create_segment(uint32_t , int );
void delete_segment(uint32_t );
//new new ones
void solicitarInstancia(char*);
void liberarInstancia(char*);

int obtenerTiempoEspera();

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION
void* serializar_contextoEjecucion(t_paquete* paquete, int bytes);
void enviar_contexto();
void enviarContextoActualizado(); 

#endif 