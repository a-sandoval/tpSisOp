#include "../include/utils.h"

t_log* logger;

int iniciar_servidorMemoria(void)
{

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	int socket_memoria = socket(servinfo->ai_family,
                    servinfo->ai_socktype,
                    servinfo->ai_protocol);

	bind(socket_memoria, servinfo->ai_addr, servinfo->ai_addrlen);

	listen(socket_memoria, SOMAXCONN);
	
	freeaddrinfo(servinfo);

	log_trace(logger, "Listo para escuchar");

	return socket_memoria;
}

int esperar_ModCliente(int socket_memoria)
{
	
	int socket_modCliente = accept(socket_memoria, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_modCliente;
}

int recibir_operacion(int socket_modCliente)
{
	int cod_op;
	if(recv(socket_modCliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_modCliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_modCliente)
{
	void * buffer;

	recv(socket_modCliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_modCliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_modCliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_modCliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_modCliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_modCliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}