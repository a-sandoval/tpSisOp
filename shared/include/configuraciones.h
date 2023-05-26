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
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>

extern int socketCliente;
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
 * @fn FILE *abrir (char *archivo, char *tipoDeArchivo)
 * Funcion hecha para abrir archivos y comprobar su apertura.
 * 
 * @param archivo Nombre del archivo a recibir, tiene que estar en la carpeta directa donde se corre el programa.
 * @param tipoDeArchivo Linea de texto del tipo de archivo para abrir, para referencia ver fopen(3).
 * @return FILE* Retorna el archivo abierto.
 */
FILE *abrir (char *archivo, char *tipoDeArchivo);

/**
 * @fn void error (char *mensajeFormato, ...)
 * Funcion de error para terminar el programa en caso de fallo.
 *  
 * @param mensajeFormato Formato del mensaje como un printf para mandarlo por log_error().
 * @param ... Resto de los parametros en caso de mandarse.
 */
void error (char *mensajeFormato, ...) __attribute__((format(printf, 1, 2)));

/**
 * @}
 */


#endif /* CONFIGURACIONES_H_ */