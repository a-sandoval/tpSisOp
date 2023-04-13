#ifndef SERVER_H_
#define SERVER_H_

#include "kernel/include/kernel.h"
#include"../../shared/include/manejoListas.h"

t_log* logger;
int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola);
void iterator(char* value);
Lista* obtenerClavesValidas(t_config* memoria_config);

#endif /* SERVER_H_ */