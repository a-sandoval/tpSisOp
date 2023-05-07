#include "memoria/include/memoria.h"

int main(){
    logger = iniciarLogger("memoria.log", "Memoria");
	loggerError = iniciarLogger("errores.log","Memoria"); 
	config = iniciarConfiguracion("memoria.config");
	

	//inicio servidor y queda a la espera de clientes
	socketCliente = alistarServidor(confGet("PUERTO_ESCUCHA"));

	//int rdoEjecucion = 
	ejecutarServidor(socketCliente);

	//if(rdoEjecucion == EXIT_FAILURE) terminarPrograma(config,logger);
	
	
	/*
	pthread_t threadCPU;
    if(!pthread_create(&threadCPU, NULL, (void*) ejecutarServidor, &cliente_fd)){
        pthread_detach(threadCPU);
	}	
    else {
        log_error(logger, "Error en iniciar el servidor a CPU");
	    terminarPrograma();
        return EXIT_FAILURE;
    }
	pthread_t threadKernel;
    if(!pthread_create(&threadKernel, NULL, (void*) ejecutarServidor, &cliente_fd)){
        pthread_detach(threadKernel);
	}	
    else {
        log_error(logger, "Error en iniciar el servidor a Kernel");
	    terminarPrograma();
        return EXIT_FAILURE;
    }
	pthread_t threadFS;
    if(!pthread_create(&threadFS, NULL, (void*) ejecutarServidor, &cliente_fd)){
        pthread_detach(threadFS);
	}	
    else {
        log_error(logger, "Error en iniciar el servidor a FS");
	    terminarPrograma();
        return EXIT_FAILURE;
    }
	*/


	//log_info(logger,"Terminando servidor");
	terminarPrograma();
	return 0;
}

void iterator(void* value) {
	log_info(logger,"%s", (char *) value);
}