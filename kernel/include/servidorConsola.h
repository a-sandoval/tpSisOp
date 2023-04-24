#ifndef SERVER_H_
#define SERVER_H_

#include <commons/log.h>
#include <commons/config.h>
#include <shared/include/utilsCliente.h>
#include <shared/include/utilsServidor.h>
#include <shared/include/configuraciones.h>
#include "kernel/include/planificacion.h"

extern t_log *logger;
extern t_config *config;

extern t_list* pcbs_new; 
extern t_list* pcbs_ready; 

int servirAConsola();
void iterator(char* value);
void obtenerClavesValidas(t_list *clavesValidas);

#endif /* SERVER_H_ */