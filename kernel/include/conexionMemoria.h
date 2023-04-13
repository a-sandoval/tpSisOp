#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include "kernel/include/kernel.h"
#include "shared/include/utilsCliente.h"

int conexionMemoria(t_config*);
void obtenerDeConfiguracionComoClienteMemoria(t_config* ,char** , char** , char** ); 
void terminar_programaMemoria(int, t_log*);

#endif /* CONEXION_MEMORIA_H */