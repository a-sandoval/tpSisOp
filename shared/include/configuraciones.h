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

extern t_log *logger;
extern t_config *config;

/**
 * @brief Código de operación utilizado para enviar al servidor para informarle que tipo de paquete se utiliza.
 *        Actualmente los únicos dos códigos que se utilizan son MENSAJE, para realizar Handshakes, y PAQUETE, para enviar información.
 */
typedef enum
{	
	MENSAJE,
	PAQUETE, 

}op_code;

/**
 * @brief Abre un archivo de configuración o retorna nulo.
 * @param ruta Ruta/nombre del archivo de configuración.
 * @return Retorna un archivo de configuración, o en caso de fallar un NULL.
 */
t_config* iniciarConfiguracion(char* ruta);

/**
 * @brief Crea un logger con los parametros indicados.
 * @param nombreArchivo Archivo donde se guardan los logs.
 * @param nombreLogger Nombre del logger.
 * @return Retorna el logger creado.
 */
t_log* iniciarLogger(char* nombreArchivo, char* nombreLogger);

t_log *cambiarNombre(char *nuevoNombre);

char *confGet(char *key);

void terminarPrograma();

#endif /* CONFIGURACIONES_H_ */