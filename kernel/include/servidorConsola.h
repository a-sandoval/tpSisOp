#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "kernel/include/kernel.h"
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<assert.h>
#include<shared/include/manejoListas.h>
#include<shared/include/utilsCliente.h>
#include<shared/include/utilsServidor.h>
#include<shared/include/configuraciones.h>

t_log* logger_sv;
int servirAConsola(t_log*, t_config* );
void iterator(char* value);
void terminar_programa_sv(t_config* ,t_log*);
void obtenerClavesValidas(t_config*, Lista* );

#endif /* SERVER_H_ */