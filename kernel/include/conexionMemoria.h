#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include "kernel/include/kernel.h"

int conexionMemoria();

void obtenerDeConfiguracionComoCliente(t_config* config_clienteMemoria,char** puerto, char** ip, char** claveHandshake); 
void terminar_programa(int conexion, t_log* logger);

#endif /* CONEXION_MEMORIA_H */