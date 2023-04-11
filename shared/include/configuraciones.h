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
 * @brief Busca valor de una configuración específica.
 * @param configuracion Archivo de configuración que buscar.
 * @param key Llave para buscar el valor.
 * @return Retorna el valor grabado en la configuración.
 */
char* obtenerStringDe(t_config* configuracion, char* key);

#endif /* CONFIGURACIONES_H_ */