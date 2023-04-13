#ifndef CONEXION_FILESYSTEM_H
#define CONEXION_FILESYSTEM_H

#include <commons/config.h>
#include "kernel/include/kernel.h"
#include "shared/include/utilsCliente.h"

int conexionFileSystem(t_config*);
void obtenerDeConfiguracionComoCliente(t_config* config_clienteFileSystem,char** puerto, char** ip, char** claveHandshake); 
//void terminar_programa(int conexion, t_log* logger);

#endif /* CONEXION_FILESYSTEM_H */