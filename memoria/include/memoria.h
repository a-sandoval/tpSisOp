#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<assert.h>
#include<shared/include/manejoListas.h>
#include<shared/include/utilsCliente.h>
#include<shared/include/utilsServidor.h>
#include<shared/include/configuraciones.h>

void iterator(char* value);

t_log* logger;
Lista* obtenerClavesValidas(t_config* memoria_config);
void terminar_programa(t_config* memoriaconfig, t_log* logger);

#endif 

