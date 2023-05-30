#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <commons/config.h>
#include "shared/include/global.h"
#include "shared/include/configuraciones.h"


extern t_config* config;

// Configuracion
/**
 * @brief Obtiene la estimacion inicial de rafaga del archivo de configuraciones del kernel
 * @param
 * @return el valor de la estimacion inicial de la rafaga
 */
double  obtenerEstimacionInicial();
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
double obtenerAlfaEstimacion(); 
char** obtenerRecursos();
char **obtenerInstanciasRecursos(); 

#endif