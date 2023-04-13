#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "kernel/include/kernel.h"
//#include"../../shared/include/manejoListas.h"
#include "shared/include/utilsServidor.h"

extern t_log* logger;
int servirAConsola(t_log*, t_config* );
void iterator(char* );
void terminar_programa_sv(t_config* ,t_log*);
//Lista* obtenerClavesValidas(t_config*);

#endif /* SERVER_H_ */