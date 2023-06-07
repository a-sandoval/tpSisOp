/* - Interprete entre el hardware y el software - */
#ifndef KERNEL_H_
#define KERNEL_H_

// Generales
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>

// Conexiones propias del kernel
#include "kernel/include/conexiones/conexionMemoria.h"
#include "kernel/include/conexiones/conexionCPU.h"
#include "kernel/include/conexiones/conexionFileSystem.h"
#include "kernel/include/conexiones/servidorConsola.h"
#include "kernel/include/planificacion/planificacion.h"
#include "kernel/include/peticiones/pcb.h"
#include "kernel/include/planificacion/algoritmosCortoPlazo.h"
#include "kernel/include/configuraciones.h"

// Utils y funciones del shared
#include "shared/include/configuraciones.h"

int socketCliente;
t_log* logger;
t_log* loggerError;
t_config* config;

#endif