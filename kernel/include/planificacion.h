#ifndef PLANIFICACION_PROCESOS_H
#define PLANIFICACION_PROCESOS_H

#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include "shared/include/utilsCliente.h"
#include <shared/include/global.h>
#include "pcb.h"

extern t_log *logger;
extern t_config *config;

int32_t procesosCreados = 0; 
t_list* pcbsNEW; 
t_list* pcbsREADY;


/**
 * @brief Obtiene la estimacion inicial de rafaga del archivo de configuraciones del kernel
 * @param
 * @return el valor de la estimacion inicial de la rafaga
 */
int obtenerEstimacionInicial();

/**
 * @brief Crea e inicializa todos los miembros de una estructura de tipo t_pcb
 * @param
 * @return un puntero a t_pcb listo para usar
 */
t_pcb* crearPCB();

/**
 * @brief Crea todas las listas de PCBs en los distintos estados
 * @param
 * @return
 */
void inicializarListasPCBS(); 

/**
 * @brief Libera la memoria de una lista de pcbs
 * @param pcbs lista de pcbs
 * @return
 */
void destruirListaPCB(t_list* pcbs);

/**
 * @brief Libera la memoria asociada a un pcb
 * @param pcb puntero a una estructura de tipo pcb
 * @return
 */
void destruirPCB(t_pcb* pcb);

/**
 * @brief Libera la memoria de una instruccion
 * @param instruccion instruccion de tipo t_instruccion
 * @return
 */
void destruirInstruccion(t_instruccion* instruccion);

/**
 * @brief obtiene el grado multiprogramacion del archivo de configuraciones
 * @param
 * @return el grado de multiprogramacion de la cpu
 */
int obtenerGradoMultiprogramacion();

/**
 * @brief Selecciona el proximo pcb a ejecutar segun el algoritmo FIFO
 * @param
 * @return el proximo puntero a t_pcb a ejecutar
 */
t_pcb* proximoAEjecutarFIFO();

/**
 * @brief Selecciona el proximo pcb a ejecutar segun el algoritmo FIFO
 * @param pcbs lista de pcbs de la cual se va a extraer el primer elemento
 * @return el proximo puntero a t_pcb a ejecutar
 */
t_pcb* desencolar(t_list* pcbs);


/**
 * @brief Selecciona el proximo pcb a ejecutar segun el algoritmo FIFO
 * @param pcbs lista de pcbs donde se encola el pcb
 * @param pcb el puntero a t_pcb a encolar en la lista de pcbs
 */
void encolar(t_list* pcbs,t_pcb* pcb);


/**
 * @brief Toma el primer pcb de la lista de pcbs_new, lo encola en la lista de pcbs_ready y lo cambia a estado READY
 */
void planificarALargoPlazo();


void planificarACortoPlazo(); 

#endif 