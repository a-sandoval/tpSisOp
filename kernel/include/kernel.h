/* - Interprete entre el hardware y el software - */
#ifndef KERNEL_H_
#define KERNEL_H_

// Generales
#include <commons/string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/config.h>
// Conexiones propias del kernel
#include "kernel/include/conexionMemoria.h"
#include "kernel/include/conexionCPU.h"
#include "kernel/include/conexionFileSystem.h"
#include "kernel/include/servidorConsola.h"
// Utils y funciones del shared
#include "shared/include/configuraciones.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/global.h"

int conexion(char *CONEXION);

#endif
