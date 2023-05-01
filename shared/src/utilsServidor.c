#include "../include/utilsServidor.h"

int iniciar_servidor(char *puerto){

	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype,servinfo->ai_protocol);

	// Asociamos el socket a un puerto
	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor){

	int socketClienteFD = accept(socket_servidor, NULL, NULL);
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

void recibir_mensaje()
{
	int size;
	char* buffer = recibir_buffer(&size);
	free(buffer);
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
void alistarServidor(char *puerto){

	int server_fd = iniciar_servidor(puerto);

	log_info(logger, "Servidor listo para recibir al cliente");

	esperar_cliente(server_fd);
}


char* recibir_clave(){
	int size;
	char* buffer = recibir_buffer(&size);

	return buffer;
}


int ejecutarServidor(){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion();
		switch (cod_op) {
			
		case MENSAJE:
			
			log_info(logger, "Se autoriza continuar");
			
			break;
		case PAQUETE:
			lista = recibir_paquete();
			list_iterate(lista, (void*) iterator); 
			list_destroy_and_destroy_elements(lista, (void*)element_destroyer);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void element_destroyer(void *palabra){
	free(palabra);
}
