#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>

int conexionMemoria();

void obtenerValoresConfig(char**handshake_uso,char**ip_uso,char**puerto_uso);

void handshake(char*valor,int conexion);

t_config* iniciar_config(void);

#endif /* CONEXION_MEMORIA_H */