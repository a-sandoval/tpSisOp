#ifndef CONTEXTO_EJECUCION_H
#define CONTEXTO_EJECUCION_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "shared/include/global.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "kernel/include/pcb.h"

typedef struct {

        t_comando comando; 
        int parametrosLength;
        char *parametros[3]; 

} t_motivoDeDesalojo; 

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
    t_motivoDeDesalojo *motivoDesalojo;
    int64_t rafagaCPUEjecutada;

} t_contexto; 

extern int conexionACPU;

void iniciarContexto();
void asignarPCBAContexto(t_pcb* proceso);
//void iterator(void *value);
void destroyContexto();


//Serializacion y deserializion del contexto
void enviarContexto();
void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones);
void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU);
void agregarMotivoAPaquete(t_paquete* paquete, t_motivoDeDesalojo* motivoDesalojo);

void recibirContextoActualizado();
void* recibirTablaDeArchivos();
void* recibirTablaDeSegmentos();


#endif