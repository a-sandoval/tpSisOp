#ifndef SERVER_H_
#define SERVER_H_

#include "kernel/include/kernel.h"

t_log* logger;
int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola);
char* obtenerDeConfiguracionComoServidor(char ** puertoEscucha, t_config* config_servidorConsola);
void iterator(char* value);

#endif /* SERVER_H_ */