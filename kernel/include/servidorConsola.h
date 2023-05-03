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


int servirAConsola();
void iterator(void* value);
void recibirConsolas(); 
void ejecutarServidorKernel(); 

/**
* @brief Inicializa todos los semaforos necesarios para que el kernel sirva a la consola
*/
void inicializarSemaforos();

#endif /* SERVER_H_ */