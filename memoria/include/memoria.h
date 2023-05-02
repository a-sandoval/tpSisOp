#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

/** 
 * @defgroup Memoria Memoria
 * @addtogroup Memoria
 * @{
 * 
 * @brief La memoria tiene el trabajo de manejar su memoria y trabajarla a pedido de los tres modulos que interactuan con ella.
 * 
 * 
 */

#include<stdio.h>
#include<stdlib.h>

#include<commons/log.h>
#include<commons/config.h>

#include "shared/include/utilsServidor.h"
#include "shared/include/configuraciones.h"

/*
#include<commons/string.h>
#include<commons/collections/list.h>
#include "shared/include/utilsCliente.h"
*/

/**
 * @var socketFD
 * Socket para recibir conexiones.
 * 
 */
int socketFD;

/**
 * @var logger 
 * Logger global para documentar todo evento que ocurra en la memoria.
 */
t_log* logger;

/**
 * @var config 
 * Archivo de configuracion utilizado para sacar informacion variada para su uso en la memoria.
 */
t_config* config;

/**
 * @fn void iterator(void* value)
 * La funcion es utilizada para ser llamada por el iterador de listas list_iterate() que se llama cuando se recibe un paquete, y segun su uso va a ser utilizado para varias funciones.
 * Actualmente unicamente se utiliza para imprimir al logger la informacion recibida.
 * 
 * @param value El valor que recibe del paquete sera recibido como parametro y utilizado como se desee.
 */
void iterator(void* value);

/**
 * Fin.
 * @}
 */
#endif 

