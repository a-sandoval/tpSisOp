#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include "kernel/include/kernel.h"

int conexionMemoria(t_config*);

void obtenerDeConfiguracionComoCliente(t_config* ,char**, char**, char**); 
void terminar_programa(int, t_log*);

#endif /* CONEXION_MEMORIA_H */