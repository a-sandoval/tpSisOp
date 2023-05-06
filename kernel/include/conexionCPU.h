#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include <commons/string.h>
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "pcb.h"

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

extern int conexionACPU;

void conexionCPU();
int  procesarPCB(t_pcb* procesoAEjecutar);

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones);
void actualizarPCB(t_pcb* proceso);
void asignarPCBAContexto(t_pcb*  proceso);

void* serializar_contextoEjecucion(t_paquete* paquete, int bytes);
void enviar_contexto();


//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU

void recibir_contexto();



#endif /* CONEXION_CPU_H */