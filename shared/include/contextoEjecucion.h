#ifndef CONTEXTO_EJECUCION_H
#define CONTEXTO_EJECUCION_H

#include "shared/include/global.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

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
    t_motivoDeDesalojo* motivoDesalojo;
    int64_t rafagaCPUEjecutada;

} t_contexto;

extern t_contexto* contextoEjecucion;
extern int socketCliente;


void agregarMotivoAPaquete(t_paquete* paquete, t_motivoDeDesalojo* motivoDesalojo);
void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU);
void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones);

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION
void enviarContextoActualizado(int socket);

//FUNCIONES PARA RECIBIR CONTEXTO DE EJECUCION
void recibirContextoActualizado(int socket);
void deserializarInstrucciones (void * buffer, int * desplazamiento);
void deserializarRegistros (void * buffer, int * desplazamiento);
void deserializarMotivoDesalojo (void * buffer, int * desplazamiento);

void iniciarContexto();
void destroyContexto();
void destroyContextoUnico();


#endif