#include "memoria/include/memoria.h"

int sockets[3];

int main() {

    logger = iniciarLogger("memoria.log", "Memoria");
	loggerError = iniciarLogger("errores.log","Memoria"); 
	config = iniciarConfiguracion("memoria.config");
	espacioDeUsuario = malloc(config_get_int_value(config,"TAM_MEMORIA")); 

	//Inicio servidor y queda a la espera de las conexiones de CPU, Kernel y File System
	
	int server_fd = iniciarServidor(confGet("PUERTO_ESCUCHA"));

	for(int i = 0; i < 3; i++) {
		sockets[i] = esperar_cliente(server_fd);
	}
	
	pthread_t threadCPU;
    if(!pthread_create(&threadCPU, NULL, (void*) ejecutarServidor, &(sockets[0]))){
        pthread_detach(threadCPU);
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a CPU");
	    terminarPrograma();
        return EXIT_FAILURE;
    }

	pthread_t threadKernel;
    if(!pthread_create(&threadKernel, NULL, (void*) ejecutarServidor, &(sockets[1]))){
        pthread_detach(threadKernel);
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a Kernel");
	    terminarPrograma();
        return EXIT_FAILURE;
    }
	
	pthread_t threadFS;
    if(!pthread_create(&threadFS, NULL, (void*) ejecutarServidor, &(sockets[2]))){
        pthread_detach(threadFS);
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a FS");
	    terminarPrograma();
        return EXIT_FAILURE;
    }

	terminarPrograma();
	return 0;
}

void iterator(void* value) {
	log_info(logger,"%s", (char *) value);
}


int ejecutarServidor(int socket){
	t_list* lista;
	while (1) {
		int codOP = recibir_operacion();
		switch (codOP) {
			case MENSAJE:
				log_info(logger, "Se autoriza continuar :)");
				break;
			case PAQUETE:
				lista = recibir_paquete();
				list_iterate(lista, (void*) iterator); 
				list_destroy_and_destroy_elements(lista, (void*)elementDestroyer);
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
