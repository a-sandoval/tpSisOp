#ifndef CPU_H
#define CPU_H

#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/configuraciones.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include<commons/collections/list.h>

int socketCliente;
t_log* logger;
t_config* config;

int conexionMemoria();
void iterator(void *value);



#endif 