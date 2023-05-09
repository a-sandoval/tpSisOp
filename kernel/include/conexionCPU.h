#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include <commons/string.h>
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "pcb.h"

typedef struct {

    uint32_t pid; 
    int programCounter;
    estadoProceso estado; 
    uint32_t instruccionesLength;
    t_list* instrucciones; 
    t_dictionary* registrosCPU;  
    uint32_t tablaDeArchivosSize;
    t_list* tablaDeArchivos; 
    uint32_t tablaDeSegmentosSize;
    t_list* tablaDeSegmentos; 

} t_contexto; 

extern int conexionACPU;
extern t_log* loggerError; 

void conexionCPU();

int procesarPCB(t_pcb* procesoEnEjecucion);

void iniciarContexto();

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION

void asignarPCBAContexto(t_pcb*  proceso);

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones);
void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU);
void enviar_contexto();


//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU
void recibirContextoActualizado();


void actualizarPCB(t_pcb* proceso);
void destroyContexto();
extern t_log* loggerError; 


#endif /* CONEXION_CPU_H */