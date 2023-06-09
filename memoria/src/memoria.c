#include "memoria/include/memoria.h"

int sockets[3];
t_segmento* segmento0; 

int main() {

    logger = iniciarLogger("memoria.log", "Memoria");
	loggerError = iniciarLogger("errores.log","Memoria"); 
	config = iniciarConfiguracion("memoria.config");
	espacioDeUsuario = malloc(config_get_int_value(config,"TAM_MEMORIA")); 
	
	segmento0 = crearSegmentoCompartido(); 

	//Inicio servidor y queda a la espera de las conexiones de CPU, Kernel y File System
	
	int server_fd = iniciarServidor(confGet("PUERTO_ESCUCHA"));

	log_info(logger, "Servidor listo para recibir clientes");
	
	log_info(logger, "Esperando CPU...");

	sockets[0] = esperarCliente(server_fd);
	
	pthread_t threadCPU;
    if(!pthread_create(&threadCPU, NULL, (void*) ejecutarServidorCPU, (void*) &sockets[0])){
		pthread_detach(threadCPU);
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a CPU");
	    terminarPrograma();
    }
	
	log_info(logger, "Esperando File System...");

	sockets[1] = esperarCliente(server_fd);

	pthread_t threadFS;
    if(!pthread_create(&threadFS, NULL, (void*)ejecutarServidorFS, (void*) &sockets[1])){
		pthread_detach(threadFS);
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a FS");
	    terminarPrograma();
        return EXIT_FAILURE;
    }
	
	log_info(logger, "Esperando Kernel...");

	sockets[2] = esperarCliente(server_fd);

	pthread_t threadKernel;
    if(!pthread_create(&threadKernel, NULL, (void*) ejecutarServidorKernel, (void*) &sockets[2])){
		pthread_join(threadKernel, NULL);
	}	
    else {
        log_error(loggerError, "Error en iniciar el servidor a Kernel");
	    terminarPrograma();
        return EXIT_FAILURE;
    }

	return 0;
}


t_segmento* crearSegmentoCompartido() {

	segmento0 = malloc(sizeof(t_segmento)); 
    segmento0->id=0; 
    segmento0->direccionBase=0; 
    segmento0->tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 

	return segmento0; 
}


