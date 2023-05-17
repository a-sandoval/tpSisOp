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

int esperar_cliente(int socketServidor){

	int socketClienteFD = accept(socketServidor, NULL, NULL);
	//log_info(logger, "Se conecto un cliente!");

	return socketClienteFD;
}

int recibir_operacion(){
	int cod_op;
	if(recv(socketCliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socketCliente);
		return -1;
	}
}

void* recibir_buffer(int* size){
	void * buffer;

	recv(socketCliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socketCliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

char *recibirMensaje()
{
	int size;
	char *buffer = recibir_buffer(&size);
	return buffer;
}


t_list* recibir_paquete(){
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size);
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

	int server_fd = iniciarServidor(puerto);

	log_info(logger, "Servidor listo para recibir al cliente");

	return esperar_cliente(server_fd);
}

void element_destroyer(void *palabra){
	free(palabra);
}

// █▀▀ █░█ █▄░█ █▀▀ █ █▀█ █▄░█ █▀▀ █▀   █▀▄ █▀▀ █▀█ █▀█ █▀▀ █▀▀ ▄▀█ █▀▄ ▄▀█ █▀
// █▀░ █▄█ █░▀█ █▄▄ █ █▄█ █░▀█ ██▄ ▄█   █▄▀ ██▄ █▀▀ █▀▄ ██▄ █▄▄ █▀█ █▄▀ █▀█ ▄█

/*

char* recibir_clave(){
	int size;
	char* buffer = recibir_buffer(&size);
	return buffer;
}


*/