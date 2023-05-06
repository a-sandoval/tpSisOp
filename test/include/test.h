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

#include <stdio.h>
#include <stdlib.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

#include "shared/include/global.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/configuraciones.h"

typedef enum estadoProceso{
    NEW, 
    READY,
    EXEC,
    BLOCK,
    SALIDA,
} estadoProceso; 

typedef struct {

    uint32_t pid; 
    estadoProceso estado; 
    int programCounter;  
    uint32_t registrosSize;
    t_dictionary* registrosCPU;  
    uint32_t instruccionesLength;
    t_list* instrucciones; 
    uint32_t tablaDeSegmentosSize;
    t_list* tablaDeSegmentos; 
    uint32_t tablaDeArchivosSize;
    t_list* tablaDeArchivos; 

} t_contexto; 

/**
 * @var socketCliente
 * Socket para recibir conexiones.
 * 
 */
extern int socketCliente;
extern t_log *logger;
extern t_config *config;

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

void ejecutarServidorTest();

t_contexto *recibirContexto();
t_contexto *newContexto();
void destroyContexto(t_contexto *contexto);

/**
 * Fin.
 * @}
 */
#endif 

