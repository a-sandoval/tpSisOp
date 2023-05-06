#ifndef PLANIFICACION_PROCESOS_H
#define PLANIFICACION_PROCESOS_H

#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include "shared/include/utilsCliente.h"
#include "pcb.h"
#include "shared/include/global.h"
#include <semaphore.h>
#include <pthread.h>
#include "conexionCPU.h"

extern t_list* pcbsNEW; 
extern t_list* pcbsREADY;

extern sem_t hayProcesosReady;
extern sem_t hayProcesosNuevos;
extern pthread_mutex_t mutexListaNew;
extern sem_t semGradoMultiprogramacion; 
extern int gradoMultiprogramacion; 

// Planificacion

/**
 * @brief Toma el primer pcb de la lista de pcbs_new, lo encola en la lista de pcbs_ready y lo cambia a estado READY
 */
void planificarALargoPlazo();

/**
 * @brief realiza la planificacion a corto plazo
 * @param proximoAEjecutar puntero a funcion que devuelve el proximo proceso a ejecutar siguiendo el algoritmo correspondiente
*/
void planificarACortoPlazo(t_pcb* (*proximoAEjecutar)());

/** 
 * @brief planificador a corto plazo segun algoritmo de planificacion extraido del archivo de configuraciones
*/
void planificarACortoPlazoSegunAlgoritmo();

/**
 * @brief Selecciona el proximo pcb a ejecutar segun el algoritmo FIFO
 * @param
 * @return el proximo puntero a t_pcb a ejecutar
 */
t_pcb* proximoAEjecutarFIFO();

/**
 * @brief indica el proximo elemento de la lista ready a ejecutar siguiendo el algoritmo HRRN
*/
 t_pcb* proximoAEjecutarHRRN();


// Semaforos
/**
* @brief Inicializa todos los semaforos necesarios para que el kernel sirva a la consola
*/
void inicializarSemaforos();

// Operaciones con t_pcb

/**
 * @brief Crea e inicializa todos los miembros de una estructura de tipo t_pcb
 * @param
 * @return un puntero a t_pcb listo para usar
 */
t_pcb* crearPCB();

/**
 * @brief Libera la memoria asociada a un pcb
 * @param pcb puntero a una estructura de tipo pcb
 * @return
 */
void destruirPCB(t_pcb* pcb);
t_dictionary* crearDiccionarioDeRegistros(); 

// Lista de PCBs

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
 * @brief se agrega un nuevo pcb al final de la lista de news 
 */
void ingresarANew(t_pcb *pcb); 

/**
 * @brief Obtiene el primer proceso en la cola de news. 
 */
t_pcb *obtenerSiguienteAReady();

/**
 * @brief Selecciona el proximo pcb a ejecutar segun el algoritmo FIFO
 * @param pcbs lista de pcbs donde se encola el pcb
 * @param pcb el puntero a t_pcb a encolar en la lista de pcbs
 */
void encolar(t_list* pcbs,t_pcb* pcb);

/**
 * @brief Selecciona el proximo pcb a ejecutar segun el algoritmo FIFO
 * @param pcbs lista de pcbs de la cual se va a extraer el primer elemento
 * @return el proximo puntero a t_pcb a ejecutar
 */
t_pcb* desencolar(t_list* pcbs);

// Configuracion

/**
 * @brief Obtiene la estimacion inicial de rafaga del archivo de configuraciones del kernel
 * @param
 * @return el valor de la estimacion inicial de la rafaga
 */
int obtenerEstimacionInicial();

/**
 * @brief obtiene el grado multiprogramacion del archivo de configuraciones
 * @param
 * @return el grado de multiprogramacion de la cpu
 */
int obtenerGradoMultiprogramacion();

/**
 * @brief obtiene del archivo de configuracion el algoritmo con que se va a realizar la planificacion a corto plazo
 * */
char* obtenerAlgoritmoPlanificacion();

// frees

/**
 * @brief Libera la memoria de una instruccion
 * @param instruccion instruccion de tipo t_instruccion
 * @return
 */
void destruirInstruccion(t_instruccion* instruccion);
void destruirRegistro(char* registro); 

#endif 