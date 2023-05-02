#ifndef UTILS_CLIENTE_H
#define UTILS_CLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include "configuraciones.h"


extern t_log *logger;
extern t_config *config;

/**
 * @struct t_buffer
 * @var t_buffer.size 
 * 	Tamaño del buffer creado.
 * @var t_buffer.stream
 * 	Stream con toda la informacion copiada.
 * @brief Información dentro de un paquete utilizado para enviar información a un servidor.
 *        Se divide en un int componiendo el tamaño del stream, y el stream en sí, que es la composición de información.
 */
typedef struct
{
	int size;
	void* stream;
} t_buffer;

/**
 * @struct t_paquete
 * @var t_paquete.codigoOperacion
 * El codigo de la informacion que se envia para que el servidor sepa que se le esta enviando.
 * Ver enum op_code
 * @brief El paquete a enviar a un servidor, se compone por el código de operación y el buffer.
 */
typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

/**
 * @fn int crearConexion(char* ip, char* puerto)
 * @brief Dados una direccion IP y un puerto, se conecta a dicha IP en dicho puerto y genera el socket de conexión para interactuar con este servidor.
 * @param ip Dirección a la que conectarse.
 * @param puerto Puerto al que solicitar un socket disponible.
 * @return Socket con la conexión realizada.
 */
int crearConexion(char* ip, char* puerto);
/**
 * @fn void enviar_mensaje(char* mensaje, int socket_cliente)
 * @brief Se envia un paquete del tipo MENSAJE que envia un string al socket solicitado.
 * @param mensaje El string a enviar.
 * @param socket_cliente El socket mediante el cual enviar el mensaje.
 */
void enviar_mensaje(char* mensaje, int socket_cliente);
/**
 * @fn t_paquete* crearPaquete(void)
 * @brief Se inicializa un paquete con código de operación PAQUETE.
 * @return Retorna el paquete antes inicializado, o -1 en caso de error.
 */
t_paquete* crearPaquete(void);
/**
 * @fn void agregarAPaquete(t_paquete* paquete, void* valor, int tamanio)
 * @brief Se agrega un void* al paquete elegido, apendeandolo al final del paquete.
 * @param paquete Paquete al que se agrega el valor.
 * @param valor Dato a añadir al paquete.
 * @param tamanio Tamaño del valor a añadir, usualmente un sizeof(valor) servira, pero en el caso de strings se puede utilizar (strlen(valor) + 1) * sizeof(int)
 */
void agregarAPaquete(t_paquete* paquete, void* valor, int tamanio);
/**
 * @fn void enviarPaquete(t_paquete* paquete, int socket_cliente)
 * @brief Se envia un paquete mediante el socket utilizado.
 * @param paquete Paquete a enviar.
 * @param socket_cliente Socket utilizado.
 */
void enviarPaquete(t_paquete* paquete, int socket_cliente);
/**
 * @fn void eliminarPaquete(t_paquete* paquete)
 * @brief Elimina un paquete y libera la memoria asociada a sus datos.
 * @param paquete El puntero al paquete a eliminar.
 */
void eliminarPaquete(t_paquete* paquete);

/**
 * @fn int conexion(char *SERVIDOR)
 * 
 * @brief Se realiza la conexion y se devuelve el socket generado, o un -1 en caso de fallar.
 * @param SERVIDOR Se envia el nombre del servidor al que se debe conectar para agarrar la informacion correcta de la configuracion y hacer la conexion.
 * @return int Se retorna el socket generado o un -1 en caso de fallar.
 */
int conexion(char *SERVIDOR);

#endif /* UTILS_H_ */