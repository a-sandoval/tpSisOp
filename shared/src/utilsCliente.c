#include "shared/include/utilsCliente.h"

/**
 * @brief Dado un paquete y la cantidad de bytes que contiene, se empaqueta la cantidad de bytes en un void,
 *        serializandolo y preparandolo para enviar a un servidor con el formato:
 *        1. codigo de operación del paquete
 *        2. tamaño del paquete
 *        3. el contenido del paquete en sí
 * @param paquete El paquete a serializar.
 * @param bytes La cantidad de bytes a serializar (usualmente sizeof(paquete)).
 * @return Se retorna el formato serializado como un void*.
 */
void *serializar_paquete(t_paquete *paquete, int bytes)
{
	void *magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	return magic;
}

int crearConexion(char *ip, char *puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
								server_info->ai_socktype,
								server_info->ai_protocol);

	if (!connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen))
	{
		freeaddrinfo(server_info);
		return socket_cliente;
	}
	else
	{
		freeaddrinfo(server_info);
		return -1;
	}
}

void enviar_mensaje(char *mensaje, int socket_cliente)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2 * sizeof(int);

	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminarPaquete(paquete);
}

//lo que hacen las funciones crear buffer y crear paquete ya lo hace la funcion enviar mensaje
/**
 * @brief Se aloca el buffer del paquete recibido.
 * @param paquete Paquete que aun no tiene el buffer inicializado.
 */
void crear_buffer(t_paquete *paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete *crearPaquete(void)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregarAPaquete(t_paquete *paquete, void *valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviarPaquete(t_paquete *paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2 * sizeof(int);
	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminarPaquete(t_paquete *paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int conexion(char *SERVIDOR)
{
	char *KEYS[] = {
		string_from_format("PUERTO_%s", SERVIDOR),
		string_from_format("IP_%s", SERVIDOR),
	};
	char *puerto = confGet(KEYS[0]);
	free(KEYS[0]);
	char *ip = confGet(KEYS[1]);
	free(KEYS[1]);
	int conexion = crearConexion(ip, puerto);
	log_info(logger, "Conexion creada: %d", conexion);

	return conexion;
}