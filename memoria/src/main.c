#include "../include/main.h"


int main(){

    
    logger = log_create("memoria.log", "Memoria", 1, LOG_LEVEL_DEBUG);

	int memoria_fd = iniciar_servidorMemoria();
	log_info(logger, "Memoria lista para recibir peticiones");
	int modCliente_fd = esperar_ModClient(memoria_fd);

	t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(modCliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(modCliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(modCliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El Modulo cliente se desconecto. Terminando servidor Memoria");
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
