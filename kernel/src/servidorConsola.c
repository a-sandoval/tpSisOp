/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(){

	signal(SIGINT, agarrarSIGINT);

	char* puertoDeEscucha = confGet("PUERTO_ESCUCHA"); 
	
	clavesValidas = list_create();

	obtenerClavesValidas();
	
	while(!pararPrograma) {
		//inicio servidor y queda a la espera de clientes
		int cliente_fd = alistarServidor(puertoDeEscucha);

		t_pcb* pcb = crearPCB(); 

		pthread_t servidorConsola;
    	if(!pthread_create(&servidorConsola, NULL,(void *) ejecutarServidor, &cliente_fd)){
    	    pthread_detach(servidorConsola);
    	}
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
    	}
	}
	//log_info(logger, "Terminando servidor");

	list_destroy(clavesValidas);

	return 0;
}

void iterator(char *value) {
    log_info(logger, value);
}

void obtenerClavesValidas(){

	list_add(clavesValidas,(void *) confGet("CLAVE_CONSOLA_KERNEL_MODO_USUARIO"));
	list_add(clavesValidas,(void *) confGet("CLAVE_CONSOLA_KERNEL_MODO_ADMIN"));
}

void agarrarSIGINT(int SIGNUM) {
	pararPrograma = 1;
}