#ifndef SERVER_H_
#define SERVER_H_

#include <commons/log.h>
#include <commons/config.h>
#include <shared/include/utilsCliente.h>
#include <shared/include/utilsServidor.h>
#include <shared/include/configuraciones.h>
#include <shared/include/global.h>
#include "planificacion.h"


extern t_log *logger;
extern t_config *config;

int servirAConsola();
void iterator(void* value);
void obtenerClavesValidas(t_list *clavesValidas);

#endif /* SERVER_H_ */