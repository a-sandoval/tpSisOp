#ifndef CONEXION_FILESYSTEM_H
#define CONEXION_FILESYSTEM_H

#include <commons/config.h>
//#include "kernel/include/kernel.h"
#include "shared/include/utilsCliente.h"

extern t_log *logger_clienteFileSystem;
int conexionFileSystem(t_config*);
void obtenerDeConfiguracionComoClienteFS(t_config* ,char** , char** , char** ); 
void terminar_programaFS(int , t_log* );

#endif /* CONEXION_FILESYSTEM_H */