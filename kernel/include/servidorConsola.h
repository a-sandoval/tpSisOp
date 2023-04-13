#ifndef SERVER_H_
#define SERVER_H_

#include "kernel/include/kernel.h"
//#include"../../shared/include/manejoListas.h"
#include "shared/include/utilsCliente.h"

t_log* logger;
int servirAConsola(t_log*, t_config* );
void iterator(char* );
//Lista* obtenerClavesValidas(t_config*);

#endif /* SERVER_H_ */