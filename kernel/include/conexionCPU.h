#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>

int conexionCPU();

void obtenerValoresConfig(char**handshake_uso,char**ip_uso,char**puerto_uso);

void handshake(char*valor,int conexion);

t_config* iniciar_config(void);

#endif /* CONEXION_CPU_H */