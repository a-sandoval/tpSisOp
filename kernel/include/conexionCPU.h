#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include <commons/string.h>
#include "shared/include/utilsCliente.h"
#include "pcb.h"

typedef struct {
//ver tipos de datos para hacer el sizeof mas legible
    uint32_t pid; 
    int socketPCB; 
    t_list* instrucciones; 
    uint32_t instruccionesLength;
    int programCounter;   
    t_reg registrosCPU;   // Puntero o no? 

    
    //TO DO: TABLA DE SEGMENTOS Y TABLA DE ARCHIVOS 

} t_contexto; 

extern t_log *logger;
extern t_config *config;
int conexionCPU();
int socketCliente;

#endif /* CONEXION_CPU_H */