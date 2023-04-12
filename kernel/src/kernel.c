/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(int argc, char *argv[]) {

	int conexion_memoria;
    int conexion_CPU;
    int conexion_FileSystem;
	int conexion_servir_consola;

	t_log* logger_clienteMemoria;
    t_log* logger_clienteCPU;
    t_log* logger_clienteFileSystem;
    t_log* logger_servidorConsola;

	t_config* config;

	/*Inicializando Loggers*/
	logger_clienteMemoria = iniciar_logger("KERNELcliente.log","Kernel-Memoria");
    logger_clienteCPU = iniciar_logger("KERNELcliente.log","Kernel-CPU");
    logger_clienteFileSystem = iniciar_logger("KERNELcliente.log","Kernel-FileSystem");
	logger_servidorConsola= iniciar_logger("KERNELservidor.log","Consola-Kernel");

	/*Inicializando los config*/
	config=iniciarConfiguracion("../kernel.config",logger_clienteMemoria);
    config=iniciarConfiguracion("../kernel.config",logger_clienteCPU);
    config=iniciarConfiguracion("../kernel.config",logger_clienteFileSystem);

	/*Conexion a memoria*/
	//conexion_memoria=conexionMemoria();

	/*Preparacion del kernel para servir a la consola*/
	conexion_servir_consola=alistarServidor(logger_servidorConsola,config_get_string_value(config,"PUERTO_ESCUCHA"));

	//ejecutarServidor(conexion_servir_kernel,logger_servidorKernel);
	//terminar_programa(conexion_memoria,logger);

    return 0;
}

void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}

/*
int main(void) {
    // KERNEL - cliente
    int conexion;
    char *ip, *puerto;
    t_log* logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_INFO);
    if (logger == NULL) {
        printf("Error al generar logger kernel.log \n");
    }
    log_info(logger, "Soy el log del kernel listo para ser usado!"); //Borrar en algún momento
    t_config* config = config_create("kernel.config");
    if (config == NULL) {
        log_error(logger, "Error al abrir kernel.config");
        log_destroy(logger);
        return 1;
    }

    //MEMORIA - servidor
    ip = config_get_string_value(config, "IP_MEMORIA");
    puerto = config_get_string_value(config, "PUERTO_MEMORIA");
    //CPU - servidor
    ip = config_get_string_value(config, "IP_CPU");
    puerto = config_get_string_value(config, "PUERTO_CPU");
    //FileSystem - servidor
    ip = config_get_string_value(config, "IP_FILESYSTEM");
    puerto = config_get_string_value(config, "PUERTO_FILESYSTEM");
    
    //Continuación de kernel cliente
    conexion = crear_conexion(ip, puerto);
    if (conexion == -1) { 
        char *mensaje_error = string_from_format("No se pudo realizar la conexion a la ip %s y puerto %s", ip, puerto);
        log_error(logger, mensaje_error);
        log_destroy(logger);
        config_destroy(config);
        return 1;
    }

    char *comando = readline("Usuario@TUKI $ ");
    log_info(logger, comando);
    string_trim(&comando);
    while (strcmp(comando, "exit")) {
        free(comando);
        comando = readline("Usuario@TUKI $ ");
        log_info(logger, comando);
        string_trim(&comando);
    }
    
    
    void terminar_programa(char* comando, t_log* logger, t_config* config){
        free(comando);
        log_destroy(logger);
        config_destroy(config);
    }
    
    free(comando);
    log_destroy(logger);
    config_destroy(config);

    //CONSOLA - cliente | KERNELL - servidor
    logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor(puerto);
	log_info(logger, "Kernel listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

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
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
*/

/* a desarrollar
	t_log* logger_cliente = iniciar_logger("kernel.log", "logger_cliente");
	t_log* logger_servidor = iniciar_logger("kernel.log", "logger_servidor");


    para crear dos sockets distintos
    int socket_cpu = crear_conexion(yada, ay), socket_memoria = crea,,, socket-_file =crear aaa

chequear que se conecte correctamente
*/