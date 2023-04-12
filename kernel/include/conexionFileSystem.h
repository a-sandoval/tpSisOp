#ifndef CONEXION_FILESYSTEM_H
#define CONEXION_FILESYSTEM_H

#include <commons/config.h>
#include "kernel/include/kernel.h"

int conexionFileSystem();

void obtenerDeConfiguracionComoCliente(t_config* config_clienteFileSystem,char** puerto, char** ip, char** claveHandshake); 

#endif /* CONEXION_FILESYSTEM_H */