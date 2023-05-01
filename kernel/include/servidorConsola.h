#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/queue.h>
#include <shared/include/utilsCliente.h>
#include <shared/include/utilsServidor.h>
//#include <shared/include/configuraciones.h>
#include "planificacion.h"
#include <pthread.h>


extern int socketCliente;
extern t_log *logger;
extern t_config *config;




int servirAConsola();
void iterator(void* value);
void recibirConsolas(); 
void ejecutarServidorKernel(); 



#endif /* SERVER_H_ */