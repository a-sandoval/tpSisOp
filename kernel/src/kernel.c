/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

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
    ip = config_get_string_value(config, "IP_FILE-SYSTEM");
    puerto = config_get_string_value(config, "PUERTO_FILE-SYSTEM");
    
    //Continuación de kernel cliente
    char *comando = readline("Usuario@TUKI $ ");
    log_info(logger, comando);
    string_trim(&comando);
    while (strcmp(comando, "exit")) {
        free(comando);
        comando = readline("Usuario@TUKI $ ");
        log_info(logger, comando);
        string_trim(&comando);
    }
    
    /*
    void terminar_programa(char* comando, t_log* logger, t_config* config){
        free(comando);
        log_destroy(logger);
        config_destroy(config);
    }*/
    
    free(comando);
    log_destroy(logger);
    config_destroy(config);

    //CONSOLA - cliente | KERNELL - servidor
    logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);

	int kernel_fd = iniciar_servidorKernel();
	log_info(logger, "Kernel listo para recibir peticiones");
	int modoKernelCliente_fd = esperar_ModoKernelClient(kernel_fd);

	t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(modoKernelCliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(modoKernelCliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(modoKernelCliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto. Terminando Kernel modo servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

    return 0;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}