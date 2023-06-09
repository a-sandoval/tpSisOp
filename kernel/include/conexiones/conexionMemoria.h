#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include "shared/include/utilsCliente.h"
#include "kernel/include/peticiones/pcb.h"


void conexionMemoria();
void recibirEstructurasInicialesMemoria(t_pcb*); 
void liberarMemoriaPCB(t_pcb* proceso);

extern t_log* loggerError; 

#endif /* CONEXION_MEMORIA_H */
