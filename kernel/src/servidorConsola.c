/* CONSOLA - cliente | KERNEL - servidor*/
#include "kernel/include/servidorConsola.h"

void recibirConsolas(char *puerto){ 
	int server_fd = iniciarServidor(puerto);
	log_info(logger, "Se genero el servidor en el socket %d.", server_fd);
	while(1){
		socketCliente = esperarCliente(server_fd);	
		ejecutarServidorKernel(); 
		sem_post(&hayProcesosNuevos);
	}
	close(server_fd);
}

t_pcb *PCB;

void iterator(void *value) { 
	list_add(PCB->instrucciones, value); 
}

void ejecutarServidorKernel(){
	
	t_list* lista;
	
	PCB = crearPCB(); 
	PCB->socketPCB=socketCliente; 
	
	while (1) {
		int cod_op = recibirOperacion(socketCliente); 
		switch (cod_op) {
			case PAQUETE:
				lista = recibirPaquete(socketCliente);
				list_iterate(lista, (void*) iterator); 
				//list_destroy_and_destroy_elements(lista, (void*)element_destroyer);
				list_destroy(lista);
				break;
			case MENSAJE:
				char *mensaje = recibirMensaje();
				log_info(logger, "%s", mensaje);
				if (!strcmp(mensaje, "Fin de instrucciones")) {
					enviarMensaje(string_from_format("%d", PCB->pid), socketCliente);
					ingresarANew(PCB);
					free(mensaje);
					return;
				}
				else {
					free(mensaje);
					break;
				}
			default:
				break;
		}
	}
}