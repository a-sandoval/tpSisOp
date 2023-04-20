/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(){

	char* puertoDeEscucha = confGet(config, "PUERTO_ESCUCHA"); 
	
	Lista* clavesValidas = malloc(sizeof(Lista));
	clavesValidas->cabeza = NULL;

	obtenerClavesValidas(config, clavesValidas);
	
	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, puertoDeEscucha);

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd, logger, clavesValidas);

	log_info(logger, "Terminando servidor");

	borrarLista(clavesValidas);

	return 0;
}

void iterator(char *value) {
    log_info(logger, value);
}

void obtenerClavesValidas(t_config* config, Lista* claves){
	insertar(claves, confGet(config, "CLAVE_CONSOLA_KERNEL_MODO_USUARIO"));
	insertar(claves, confGet(config, "CLAVE_CONSOLA_KERNEL_MODO_ADMIN"));
}