/* CONSOLA - cliente | KERNEL - servidor*/
#include "kernel/include/servidorConsola.h"

t_pcb* pcb; 


int servirAConsola(){

	char* puertoDeEscucha = confGet("PUERTO_ESCUCHA"); 
	
	t_list* clavesValidas = list_create();

	obtenerClavesValidas(clavesValidas);
	
	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(puertoDeEscucha);

	pcb = crearPCB(); 

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd, clavesValidas);

	//log_info(logger, "Terminando servidor");

	list_destroy(clavesValidas);

	return 0;
}

void iterator(void *value) {
    
	list_add(pcb->instrucciones,value); 


}

void obtenerClavesValidas(t_list* clavesValidas){

	list_add(clavesValidas,(void *) confGet("CLAVE_CONSOLA_KERNEL_MODO_USUARIO"));
	list_add(clavesValidas,(void *) confGet("CLAVE_CONSOLA_KERNEL_MODO_ADMIN"));
}