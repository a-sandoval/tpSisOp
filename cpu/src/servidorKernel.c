#include "../include/servidorKernel.h"

int alistarServidor(){
	char*puerto_escucha;

	logger = log_create("CPUservidor.log", "CPUservidor", 1, LOG_LEVEL_DEBUG);

	obtenerPuertoEscucha(&puerto_escucha);
	printf("Puerto Escucha: %s",puerto_escucha);

	int server_fd = iniciar_servidor(puerto_escucha);

	log_info(logger, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd);

	free(puerto_escucha);

	return cliente_fd;
}

int ejecutarServidor(int cliente_fd){
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
			list_iterate(lista, (void*) iterator);
			list_destroy_and_destroy_elements(lista, (void*)element_destroyer);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void element_destroyer(char*palabra){
	free(palabra);
}

t_config* iniciar_config_server(void){

	t_config* nuevo_config = config_create("cpu.config");

	if(nuevo_config == NULL){
		printf("No se pudo crear el config del server");
		exit(2);
	}

	return nuevo_config;
}


void obtenerPuertoEscucha(char**puerto_escucha){

	char* puerto;

	t_config* config;

	config = iniciar_config_server();

	puerto = config_get_string_value(config,"PUERTO_ESCUCHA");

	*puerto_escucha=malloc(sizeof(char)*(strlen(puerto) + 1));

	strcpy(*puerto_escucha,puerto);

	config_destroy(config);
}