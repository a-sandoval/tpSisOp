#ifndef UTILS_CLIENTE_H
#define UTILS_CLIENTE_H

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

/**
 * @brief Código de operación utilizado para enviar al servidor para informarle que tipo de paquete se utiliza.
 *        Actualmente los únicos dos códigos que se utilizan son MENSAJE, para realizar Handshakes, y PAQUETE, para enviar información.
 */
typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

/**
 * @brief Información dentro de un paquete utilizado para enviar información a un servidor.
 *        Se divide en un int componiendo el tamaño del stream, y el stream en sí, que es la composición de información.
 */
typedef struct
{
	int size;
	void* stream;
} t_buffer;

/**
 * @brief El paquete a enviar a un servidor, se compone por el código de operación y el buffer.
 */
typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

/**
 * @brief Dados una direccion IP y un puerto, se conecta a dicha IP en dicho puerto y genera el socket de conexión para interactuar con este servidor.
 * @param ip Dirección a la que conectarse.
 * @param puerto Puerto al que solicitar un socket disponible.
 * @return Socket con la conexión realizada.
 */
int crear_conexion(char* ip, char* puerto);
/**
 * @brief Se envia un paquete del tipo MENSAJE que envia un string al socket solicitado.
 * @param mensaje El string a enviar.
 * @param socket_cliente El socket mediante el cual enviar el mensaje.
 */
void enviar_mensaje(char* mensaje, int socket_cliente);
/**
 * @brief Se inicializa un paquete con código de operación PAQUETE.
 * @return Retorna el paquete antes inicializado.
 */
t_paquete* crear_paquete(void);
/**
 * @brief Se agrega un void* al paquete elegido, apendeandolo al final del paquete.
 * @param paquete Paquete al que se agrega el valor.
 * @param valor Dato a añadir al paquete.
 * @param tamanio Tamaño del valor a añadir, usualmente un sizeof(valor) servira, pero en el caso de strings se puede utilizar (strlen(valor) + 1) * sizeof(int)
 */
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
/**
 * @brief Se envia un paquete mediante el socket utilizado.
 * @param paquete Paquete a enviar.
 * @param socket_cliente Socket utilizado.
 */
void enviar_paquete(t_paquete* paquete, int socket_cliente);
/**
 * @brief Libera una conexión de un socket cliente dado.
 * @param socket_cliente El número de socket del cliente a liberar.
 */
void liberar_conexion(int socket_cliente);
/**
 * @brief Elimina un paquete y libera la memoria asociada a sus datos.
 * @param paquete El puntero al paquete a eliminar.
 */
void eliminar_paquete(t_paquete* paquete);

#endif /* UTILS_CLIENTE_H */
