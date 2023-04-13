#ifndef CPU_H
#define CPU_H

#include <commons/config.h>
#include <commons/log.h>
#include "../../shared/include/manejoListas.h"
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsCliente.h"
#include "../../shared/include/utilsServidor.h"

void terminar_programa(int conexion, t_log* loggerCliente,t_log* loggerServidor,t_config* config,Lista*lista);
int conexionMemoria(t_config* config);
void iterator(char *value);

#endif 