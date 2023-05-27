#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include <commons/string.h>
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "pcb.h"
#include "shared/include/global.h"


typedef struct {

        t_comando comando; 
        int parametrosLength;
        char* parametros[3]; 

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
extern t_log* loggerError; 

void conexionCPU();

int procesarPCB(t_pcb* procesoEnEjecucion);

void iniciarContexto();

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION

t_dictionary *registrosDelCPU(t_dictionary *aCopiar);
void asignarPCBAContexto(t_pcb*  proceso);

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones);
void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU);
void agregarMotivoAPaquete(t_paquete* paquete, t_motivoDeDesalojo* motivoDesalojo);
void enviarContexto();


//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU
void recibirContextoActualizado();


void actualizarPCB(t_pcb* proceso);
void destroyContexto();
extern t_log* loggerError; 


#endif /* CONEXION_CPU_H */