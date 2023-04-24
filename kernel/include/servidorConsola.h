#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <commons/log.h>
#include <commons/config.h>
#include <shared/include/utilsCliente.h>
#include <shared/include/utilsServidor.h>
#include <shared/include/configuraciones.h>
#include "planificacion.h"
#include <pthread.h>

extern t_log *logger;
extern t_config *config;
extern t_list *clavesValidas;
extern volatile sig_atomic_t pararPrograma;

int servirAConsola();
void iterator(char* value);
void obtenerClavesValidas();
void agarrarSIGINT(int SIGNUM);

#endif /* SERVER_H_ */