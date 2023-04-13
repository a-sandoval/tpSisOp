#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include "kernel/include/kernel.h"
#include "shared/include/utilsCliente.h"

int conexionCPU(t_config*);
void terminar_programaCPU(int , t_log* );
void obtenerDeConfiguracionComoClienteCPU(t_config* ,char** , char** , char** ); 
#endif /* CONEXION_CPU_H */