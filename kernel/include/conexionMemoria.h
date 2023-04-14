#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include <commons/string.h>
#include <shared/include/utilsCliente.h>

extern t_log *logger_clienteMemoria;
int conexionMemoria(t_config *);
void obtenerDeConfiguracionComoClienteMemoria(t_config *, char **, char **, char **);
void terminar_programaMemoria(int, t_log *);

#endif /* CONEXION_MEMORIA_H */