#ifndef CONTEXTO_EJECUCION_H
#define CONTEXTO_EJECUCION_H

#include "shared/include/global.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "stdlib.h"
#include "stdint.h"

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


void agregarMotivoAPaquete(t_paquete* paquete, t_motivoDeDesalojo* motivoDesalojo);
void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU);

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION
void* serializar_contextoEjecucion(t_paquete* paquete, int bytes);
void enviar_contexto();
void enviarContextoActualizado();

//FUNCIONES PARA RECIBIR CONTEXTO DE EJECUCION
void recibirContextoActualizado();
void iniciarContexto();
void iterator(void*);
void destroyContexto();


#endif