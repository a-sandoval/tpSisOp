/* - Interprete entre el hardware y el software - */
#ifndef KERNEL_H_
#define KERNEL_H_

// Generales
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include <commons/collections/list.h>
#include<readline/readline.h>
// Conexiones propias del kernel
#include "../include/conexionMemoria.h"
#include "../include/conexionCPU.h"
#include "../include/conexionFileSystem.h"
#include "../include/servidorConsola.h"
// Utils y funciones del shared
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsServidor.h"
#include "../../shared/include/utilsCliente.h"

/* Creo que no hace falta esto, si funca lo borro
Por ser servidor
void iterator(char* value);

Por ser cliente
//void leer_consola(t_log*); ?
void terminar_programa(char* , t_log*, t_config*); //Useless ahora mismo
*/

#endif /* CLIENT_H_ */