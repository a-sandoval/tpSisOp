/* - Interprete entre el hardware y el software - */
#ifndef KERNEL_H_
#define KERNEL_H_

// Generales
#include<commons/string.h>
#include<readline/readline.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include <commons/config.h>
// Conexiones propias del kernel
#include "../include/conexionMemoria.h"
#include "../include/conexionCPU.h"
#include "../include/conexionFileSystem.h"
#include "../include/servidorConsola.h"
// Utils y funciones del shared
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsServidor.h"
#include "../../shared/include/utilsCliente.h"
#include"../../shared/include/manejoListas.h"

/* 
Por ser cliente
//void leer_consola(t_log*); ?
void terminar_programa(char* , t_log*, t_config*); //Useless ahora mismo
*/

#endif