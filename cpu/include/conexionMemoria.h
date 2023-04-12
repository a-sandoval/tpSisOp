
#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../include/cpu.h"
#include "../include/servidorKernel.h"
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsServidor.h"
#include "../../shared/include/utilsCliente.h"

int conexionMemoria();

void obtenerValoresConfig(char**handshake_uso,char**ip_uso,char**puerto_uso);

void handshake(char*valor,int conexion);

t_config* iniciar_config(void);

#endif /* CONEXION_MEMORIA_H */
