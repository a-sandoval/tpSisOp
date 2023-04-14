#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <commons/config.h>
#include <commons/string.h>
// #include "kernel/include/kernel.h"
#include "shared/include/utilsCliente.h"

extern t_log *logger_clienteCPU;
int conexionCPU(t_config *);

#endif /* CONEXION_CPU_H */