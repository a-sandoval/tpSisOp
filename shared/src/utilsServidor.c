#include "../include/utilsServidor.h"

//Se le agrego el parametro logger, para que funciones
int iniciar_servidor(char *puerto,t_log* logger){

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

//Se agrego Logger como parametro
int esperar_cliente(int socket_servidor,t_log* logger){

	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente){
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente){
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente){
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
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

/*Funcione con printf ?? es para no tener una variable externa todos.
void iterator(char* value) {
	log_info(logger,"%s", value);
}*/

void element_destroyer(char*palabra){
	free(palabra);
}

//FUNCIONES DE USO COLECTIVO PARA EL SERVIDOR:
int alistarServidor(t_log *logger, char *puerto){

	int server_fd = iniciar_servidor(puerto,logger);

	log_info(logger, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd,logger);

	return cliente_fd;
}


char* recibir_clave(int socket_cliente){
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);

	return buffer;
}

int ejecutarServidor(int cliente_fd, t_log* logger, Lista* clavesValidas){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			char* claveRecibida = recibir_clave(cliente_fd);
			bool claveValida = esClaveValida(clavesValidas, claveRecibida);
			
			if(!claveValida){
				log_error(logger, "Cliente no reconocido"); // quien sos flaco?
				return EXIT_FAILURE;
			}
			log_info(logger, "Clave valida, autorizo informacion");
			free(claveRecibida); 
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n"); 
			//list_iterate(lista, (void*) iterator); NO ANDA EL ITERATOR SIN EL LOGGER VARIABLE GLOBAL
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

/* Llamado a la funcion alistarServidor
alistarServidor(logger, config_get_string_value(config,"PUERTO_ESCUCHA"));
*/

int ejecutarServidorOriginal(int cliente_fd, t_log* logger){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n"); 
			//list_iterate(lista, (void*) iterator); No permite pasarle el logger como patametro
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
