#ifndef PLANIFICACION_PROCESOS_H
#define PLANIFICACION_PROCESOS_H

#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include "shared/include/utilsCliente.h"
#include "cpu/include/registros.h""

extern t_log *logger;
extern t_config *config;

typedef enum {

    NEW, 
    READY, 
    EXEC, 
    BLOCK, 
    EXIT,
} estadoProceso; 

int32_t procesosCreados = 0; 
t_list* pcbs_new; 
t_list* pcbs_ready; 

typedef struct t_pcb {

    int32_t pid; 
    t_list* instrucciones; 
    int programCounter;   
    estadoProceso estado; 
    t_reg* registrosCPU;   // Puntero o no? 
    int estimadoProximaRafaga; 
    t_temporal* llegadaAReady; 

    //TO DO: TABLA DE SEGMENTOS Y TABLA DE ARCHIVOS 


} t_pcb; 


int obtenerEstimacionInicial(); 
t_pcb crearPCB(); 

#endif 
