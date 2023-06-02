#ifndef MEMORIA_CONEXION_CPU_H
#define MEMORIA_CONEXION_CPU_H

#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "shared/include/utilsServidor.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/global.h"


int ejecutarServidorCPU(int * socket); 


extern int socketCliente;
extern t_config* config;
extern t_log* logger; 



#endif 



