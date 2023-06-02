#include "shared/include/utilsServidor.h"

int iniciarServidor(char *puerto){

	int socketServidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socketServidor = socket(servinfo->ai_family, servinfo->ai_socktype,servinfo->ai_protocol);
	int temp = 1;
	setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &(temp), sizeof(int));

	// Asociamos el socket a un puerto
	bind(socketServidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socketServidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socketServidor;
}

int esperarCliente(int socketServidor){

	int socketClienteFD = accept(socketServidor, NULL, NULL);
	//log_info(logger, "Se conecto un cliente!");

	return socketClienteFD;
}

int recibirOperacion(int socketCliente){
	int cod_op;
	if(recv(socketCliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socketCliente);
		return -1;
	}
}

void* recibirBuffer(int socket, int  * size){
	void * buffer;

	recv(socket, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket, buffer, *size, MSG_WAITALL);

	return buffer;
}

char *recibirMensaje(int socketCliente)
{
	int size;
	char *buffer = recibirBuffer(socketCliente, &size);
	return buffer;
}

t_list* recibirPaquete(int socketCliente){
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibirBuffer(socketCliente, &size);
	while(desplazamiento < size){
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


//FUNCIONES DE USO COLECTIVO PARA EL SERVIDOR:
int alistarServidor(char *puerto){

	int serverFD = iniciarServidor(puerto);

	log_info(logger, "Servidor listo para recibir al cliente");

	return esperarCliente(serverFD);
}

void elementDestroyer(void *palabra){
	free(palabra);
}