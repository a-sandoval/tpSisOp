#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include "shared/include/utilsCliente.h"
#include "pcb.h"


int conexionMemoria();
void recibirEstructurasInicialesMemoria(t_pcb*); 
extern t_log* loggerError; 

#endif /* CONEXION_MEMORIA_H */
