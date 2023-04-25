#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>

#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/configuraciones.h"
#include "shared/include/global.h"

void iterator(void* value);
/**
 * @brief Obtiene las claves validas para conexion entre modulos del archivo de configuracion

 * @param claves Lista de claves Validas.
 * @return
 */
void obtenerClavesValidas();
/**
 * @brief 

 * @return
 */
void terminarPrograma();

#endif 

