/* - Interprete entre el hardware y el software - */
#ifndef KERNEL_H_
#define KERNEL_H_

// Generales
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>

// Conexiones propias del kernel
#include "kernel/include/conexionMemoria.h"
#include "kernel/include/conexionCPU.h"
#include "kernel/include/conexionFileSystem.h"
#include "kernel/include/servidorConsola.h"
#include "kernel/include/planificacion.h"
#include "kernel/include/pcb.h"
#include "kernel/include/algoritmosCortoPlazo.h"
#include "kernel/include/configuraciones.h"

// Utils y funciones del shared
#include "shared/include/configuraciones.h"

int socketCliente;
t_log* logger;
t_log* loggerError; 
t_config* config;

#endif
