
#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/collections/list.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsServidor.h"
#include "../../shared/include/utilsCliente.h"

int conexionMemoria(t_config* config);

void handshake(char*valor,int conexion);

#endif /* CONEXION_MEMORIA_H */
