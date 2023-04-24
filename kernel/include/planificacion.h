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
    EXIT
} estadoProceso; 


typedef struct t_pcb {

    int id;
    t_list* instrucciones = list_create(); 
    int programCounter;   
    t_reg* registrosCPU;   // Puntero o no? 
    int estimadoProximaRafaga = obtenerEstimacionInicial(); // en miliseg
    t_temporal* llegadaAReady = temporal_create(); 

    //TO DO: TABLA DE SEGMENTOS Y TABLA DE ARCHIVOS + contexto de ejecucion


} t_pcb; 


int obtenerEstimacionInicial(); 

#endif 
