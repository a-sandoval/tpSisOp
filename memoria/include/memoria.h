#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

/** 
 * @defgroup Memoria Memoria
 * @addtogroup Memoria
 * @{
 * 
 * @authors To The End
 * 
 * @brief La memoria tiene el trabajo de manejar su memoria y trabajarla a pedido de los tres modulos que interactuan con ella.
 * 
 * @file memoria.h
 * 
 */

#include<stdio.h>
#include<stdlib.h>

#include<commons/log.h>
#include<commons/config.h>
#include <pthread.h>

#include "shared/include/global.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/configuraciones.h"
#include "shared/include/utilsCliente.h"
#include "conexionKernel.h"
#include "conexionCPU.h"
#include "conexionFS.h"

/**
 * @var socketCliente
 * Socket para recibir conexiones.
 * 
 */
int socketCliente;

/**
 * @fn void iterator (void* value)
 * 
 * @brief Funcion llamada por list_iterate para iterar los valores de la lista.
 * 
 * La funcion es utilizada para ser llamada por el iterador de listas list_iterate() que se llama cuando se recibe un paquete, y segun su uso va a ser utilizado para varias funciones.
 * Actualmente unicamente se utiliza para imprimir al logger la informacion recibida.
 * 
 * @param value El valor que recibe del paquete sera recibido como parametro y utilizado como se desee.
 */

void iterator(void* value);
t_log* logger; 
t_log* loggerError; 
t_config* config; 
void* espacioDeUsuario; 

extern int sockets[3];

int ejecutarServidor(int socket); 

/**
 * Fin.
 * @}
 */

#endif
