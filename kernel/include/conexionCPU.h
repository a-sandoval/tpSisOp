#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include "kernel/include/kernel.h"

int conexionCPU();

void obtenerDeConfiguracionComoCliente(t_config* config_clienteCPU,char** puerto, char** ip, char** claveHandshake); 

#endif /* CONEXION_CPU_H */