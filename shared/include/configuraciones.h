#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

/**
 * @addtogroup Shared
 * @{
 * 
 * @file configuraciones.h
 * @authors To The End
 * 
 * @brief Archivo con funciones de las estructuras t_config y t_log.
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>

extern t_log *logger;
extern t_config *config;

/**
 * @ingroup funcionesDeConfig 
 * @{
 */


/**
 * @enum op_code
 * Codigo de operacion utilizado para dar a conocer que se esta enviando. Esto puede ser:
 * 
 * - Un mensaje simple de texto.
 * - Un paquete con uno o mas elementos.
 * - Un contexto de ejecucion.
 * 
 * @brief Código de operación utilizado para enviar al servidor para informarle que tipo de paquete se utiliza.
 *        Actualmente los únicos dos códigos que se utilizan son MENSAJE, para realizar Handshakes, y PAQUETE, para enviar información.
 */
typedef enum
{	
	MENSAJE,
	PAQUETE, 
	CONTEXTOEJECUCION
}op_code;

/**
 * @fn t_config* iniciarConfiguracion(char* ruta)
 * @brief Abre un archivo de configuración o retorna nulo.
 * @param ruta Ruta/nombre del archivo de configuración.
 * @return Retorna un archivo de configuración, o en caso de fallar un NULL.
 */
t_config* iniciarConfiguracion(char* ruta);

/**
 * @fn char *confGet(char *key)
 * 
 * @brief Alias de la funcion config_get_string_value(char *key), recortado por uso.
 * 
 * @param key 
 * Llave utilizada para buscar en la configuracion el archivo.
 * @return char* 
 * Retorna la configuracion encontrada o un NULL si no lo encuentra.
 * 
 */
char *confGet(char *key);

/**
 * 
 * @}
 * 
 * @ingroup funcionesDeLogger
 * @{
 */

/**
 * @fn t_log* iniciarLogger(char* nombreArchivo, char* nombreLogger)
 * @brief Crea un logger con los parametros indicados.
 * @param nombreArchivo Archivo donde se guardan los logs.
 * @param nombreLogger Nombre del logger.
 * @return Retorna el logger creado.
 */
t_log* iniciarLogger(char* nombreArchivo, char* nombreLogger);

/**
 * @fn t_log *cambiarNombre(char *nuevoNombre)
 * @brief Se agarra un t_log, se le modifica el nombre y se envia el logger modificado commo resultado (El anterior nombre se elimina).
 * @param nuevoNombre 
 * El nombre que va a reemplazar el anterior.
 * @return t_log* 
 * Se retorna el logger modificado.
 */
t_log *cambiarNombre(char *nuevoNombre);

/**
 * @}
 * @ingroup funcionesGlobales
 * @{
 */

/**
 * @fn void terminarPrograma()
 * @brief Programa utilizado para eliminar tanto el Config global como el Logger global en terminacion del programa.
 * 
 */
void terminarPrograma();

/**
 * @}
 */


#endif /* CONFIGURACIONES_H_ */