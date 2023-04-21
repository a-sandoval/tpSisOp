#include "memoria/include/memoria.h"

int main(){

    logger = iniciarLogger("memoria.log", "Memoria");
	config = iniciarConfiguracion("memoria.config");

	t_list* clavesValidas = list_create();
	
	obtenerClavesValidas(clavesValidas);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(confGet("PUERTO_ESCUCHA"));

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd,  clavesValidas);

	//if(rdoEjecucion == EXIT_FAILURE) terminarPrograma(config,logger);
	
	log_info(logger,"Terminando servidor");
	close(cliente_fd);

	terminarPrograma(clavesValidas);
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