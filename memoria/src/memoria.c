#include "memoria/include/memoria.h"

int sockets[3];

int main() {

    logger = iniciarLogger("memoria.log", "Memoria");
	loggerError = iniciarLogger("errores.log","Memoria"); 
	config = iniciarConfiguracion("memoria.config");
	espacioDeUsuario = malloc(config_get_int_value(config,"TAM_MEMORIA")); 

	//Inicio servidor y queda a la espera de las conexiones de CPU, Kernel y File System
	
	int server_fd = iniciarServidor(confGet("PUERTO_ESCUCHA"));
	log_info(logger, "Servidor listo para recibir al cliente");

	
	sockets[0] = esperarCliente(server_fd);
	
	pthread_t threadCPU;
    if(!pthread_create(&threadCPU, NULL, (void*) ejecutarServidorCPU, (void*)sockets[0])){
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a CPU");
	    terminarPrograma();
    }

	sockets[1] = esperarCliente(server_fd);

	pthread_t threadFS;
    if(!pthread_create(&threadFS, NULL, (void*) ejecutarServidorFS, (void*) sockets[1])){
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a FS");
	    terminarPrograma();
        return EXIT_FAILURE;
    }

	sockets[2] = esperarCliente(server_fd);

	pthread_t threadKernel;
    if(!pthread_create(&threadKernel, NULL, (void*) ejecutarServidorKernel, (void*)sockets[2])){
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a Kernel");
	    terminarPrograma();
        return EXIT_FAILURE;
    }


	pthread_join(threadCPU,NULL);
	pthread_join(threadFS, NULL);
	pthread_join(threadKernel, NULL);


	log_info(logger, "muero");
	return 0;
}


