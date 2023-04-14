#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include <commons/string.h>
#include "../../shared/include/utilsCliente.h"

extern t_log *logger_clienteMemoria;
int conexionMemoria(t_config *);

#endif /* CONEXION_MEMORIA_H */