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
#include <shared/include/configuraciones.h>
#include <shared/include/global.h>
#include "planificacion.h"
#include <pthread.h>
#include <semaphore.h>

extern int socketClienteFD;
extern t_log *logger;
extern t_config *config;
extern t_list *clavesValidas;
//extern volatile sig_atomic_t pararPrograma;

int servirAConsola();
void iterator(void* value);
void obtenerClavesValidas();
//void agarrarSIGINT(int SIGNUM);

#endif /* SERVER_H_ */