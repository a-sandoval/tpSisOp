/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(){

	char* puertoDeEscucha = confGet(config, "PUERTO_ESCUCHA"); 
	
	t_list* clavesValidas = list_create();

	obtenerClavesValidas(config, clavesValidas);
	
	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, puertoDeEscucha);

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd, logger, clavesValidas);

	log_info(logger, "Terminando servidor");

	list_destroy_and_destroy_elements(clavesValidas, element_destroyer);

	return 0;
}

void iterator(char *value) {
    log_info(logger, value);
}

void obtenerClavesValidas(t_config* config, t_list* clavesValidas){

	list_add(clavesValidas,(void *) confGet(config, "CLAVE_CONSOLA_KERNEL_MODO_USUARIO"));
	list_add(clavesValidas,(void *) confGet(config, "CLAVE_CONSOLA_KERNEL_MODO_ADMIN"));
}