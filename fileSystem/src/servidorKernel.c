#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {
    char* puertoEscucha = confGet ("PUERTO_ESCUCHA");
    socketCliente = alistarServidor (puertoEscucha);
    ejecutarServidor();  
}

void ejecutarServidor() {
	t_list* lista;
	
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
				char *mensaje = recibirMensaje(socketCliente);
				log_info(logger, "%s", mensaje);
				free(mensaje);
                enviarMensaje("Finalizado", socketCliente);
				break;
            case -1:
                close(socketCliente);
                return;
			default:
				break;
		}
	}
}

void iterator (void *value) {
    log_info(logger, "Se recibio esta data: %s.", (char *) value);
    /*
    t_paquete *paquete = crearPaquete();
    agregarAPaquete ( paquete, value, sizeof(char) * strlen((char *) value) + 1 );
    enviarPaquete(paquete, socketMemoria);
    eliminarPaquete(paquete);
    */
}