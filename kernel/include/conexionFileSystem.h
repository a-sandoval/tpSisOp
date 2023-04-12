#ifndef CONEXION_FILESYSTEM_H
#define CONEXION_FILESYSTEM_H

#include <commons/config.h>
#include "kernel/include/kernel.h"

int conexionFileSystem();

void obtenerValoresConfig(char**handshake_uso,char**ip_uso,char**puerto_uso);

void handshake(char*valor,int conexion);

t_config* iniciar_config(void);

#endif /* CONEXION_FILESYSTEM_H */