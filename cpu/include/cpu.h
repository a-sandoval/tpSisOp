#ifndef CPU_H
#define CPU_H

#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/manejoListas.h"
#include "shared/include/configuraciones.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/global.h"
#include<commons/collections/list.h>

void terminarPrograma(int conexion,t_list* lista);
int conexionMemoria();
void iterator(char *value);

#endif 