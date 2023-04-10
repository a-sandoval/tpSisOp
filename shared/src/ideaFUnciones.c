/*

t_log* iniciar_logger(char* nombreArchivo)
{
	t_log* nuevo_logger = log_create(nombreLog, "Log",1,LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger");
		exit(1);
	}

	return nuevo_logger;
}


t_log* iniciarLogger(char* ruta) {

    t_log* logger = log_create(ruta,"loggeandoAndo",0,LOG_LEVEL_INFO); 

    return logger; 
}

int alistarServidor(char* logServidor, char* servidor, char* serverConfig){
	char*puerto_escucha;

	logger = log_create(logServidor, servidor, 1, LOG_LEVEL_DEBUG);

	obtenerPuertoEscucha(&puerto_escucha, *serverConfig);
	printf("Puerto Escucha: %s",puerto_escucha);

	int server_fd = iniciar_servidor(puerto_escucha);

	log_info(logger, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd);

	free(puerto_escucha);

	return cliente_fd;
}

t_config* iniciar_config_server(char* serverConfig){

	t_config* nuevo_config = config_create(serverConfig);

	if(nuevo_config == NULL){
		printf("No se pudo crear el config del server");
		exit(2);
	}

	return nuevo_config;
}

void obtenerPuertoEscucha(char**puerto_escucha, char* serverConfig){

	char* puerto;

	t_config* config;

	config = iniciar_config_server(serverConfig);

	puerto = config_get_string_value(config,"PUERTO_ESCUCHA");

	*puerto_escucha=malloc(sizeof(char)*(strlen(puerto) + 1));

	strcpy(*puerto_escucha,puerto);

	config_destroy(config);
}

int ejecutarServidor(int cliente_fd, loggerCorrespondienteAlModulo){
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


*/