#ifndef SERVER_H_
#define SERVER_H_

#include <commons/log.h>
#include <commons/config.h>
#include <shared/include/manejoListas.h>
#include <shared/include/utilsCliente.h>
#include <shared/include/utilsServidor.h>
#include <shared/include/configuraciones.h>

extern t_log *logger;
extern t_config *config;
int servirAConsola();
void iterator(char* value);
void obtenerClavesValidas(t_config *config, Lista *claves);

#endif /* SERVER_H_ */