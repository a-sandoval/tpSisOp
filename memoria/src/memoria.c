#include "memoria/include/memoria.h"

int main(){

    logger = iniciarLogger("memoria.log", "Memoria");
	config = iniciarConfiguracion("memoria.config");

	t_list* clavesValidas = list_create();
	
	obtenerClavesValidas(clavesValidas);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(confGet("PUERTO_ESCUCHA"));

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd);

	//if(rdoEjecucion == EXIT_FAILURE) terminarPrograma(config,logger);
	
	
	/*
	pthread_t threadCPU;
    if(!pthread_create(&threadCPU, NULL, (void*) ejecutarServidor, &cliente_fd)){
        pthread_detach(threadCPU);
	}	
    else {
        log_error(logger, "Error en iniciar el servidor a CPU");
        return EXIT_FAILURE;
    }

	pthread_t threadKernel;
    if(!pthread_create(&threadKernel, NULL, (void*) ejecutarServidor, &cliente_fd)){
        pthread_detach(threadKernel);
	}	
    else {
        log_error(logger, "Error en iniciar el servidor a Kernel");
        return EXIT_FAILURE;
    }

	pthread_t threadFS;
    if(!pthread_create(&threadFS, NULL, (void*) ejecutarServidor, &cliente_fd)){
        pthread_detach(threadFS);
	}	
    else {
        log_error(logger, "Error en iniciar el servidor a FS");
        return EXIT_FAILURE;
    }
	*/

	terminarPrograma(clavesValidas);

	log_info(logger,"Terminando servidor");
	close(cliente_fd);

	return 0;
    
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}



void obtenerClavesValidas(t_list* clavesValidas){

	list_add(clavesValidas,(void *) confGet("CLAVE_CPU_MEMORIA"));
	list_add(clavesValidas,(void *) confGet("CLAVE_KERNEL_MEMORIA"));
	list_add(clavesValidas,(void *) confGet("CLAVE_FS_MEMORIA"));
	
}