#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/config.h>

/**
 * @brief Abre un archivo de configuración o retorna nulo.
 * @param ruta Ruta/nombre del archivo de configuración.
 * @param logger Logger para preparar en caso de que se falle la apertura del programa.
 * @return Retorna un archivo de configuración, o en caso de fallar un NULL.
 */
t_config* iniciarConfiguracion(char* ruta, t_log* logger);

/**
 * @brief Abre un logger.
 * @param nombreArchivo Nombre del archivo en el directorio de ejecucion o subdirectorios.
 * @param nombreModulo Nombre que aparecera en el log.
 * @return Retorna el logger inicializado.
 */
t_log* iniciarLogger(char* nombreArchivo, char* nombreModulo);

#endif /* CONFIGURACIONES_H_ */