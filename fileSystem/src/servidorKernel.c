#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {
    char* puertoEscucha = confGet ("PUERTO_ESCUCHA");
    socketCliente = alistarServidor (puertoEscucha);
	log_info(logger, "Se conecto el Kernel");
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
				log_info(logger, "Se desconecto el Kernel");
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

void recibirInstruccion () {
	usleep (tiempoDeEspera * 1000);
	switch (instruccion) {
		case F_OPEN:
			fcb_t * nuevoArchivo = abrirArchivo (nombreArchivo);
			if (nuevoArchivo == NULL) {
				if (crearArchivo(nombreArchivo) < 0)
					return;
				log_info (logger, "Crear Archivo: <%s>", nombreArchivo);
				nuevoArchivo = abrirArchivo (nombreArchivo);
			}
			log_info (logger, "Abrir Archivo: <%s>", nombreArchivo);
			enviarArchivo ();
			break;
		case F_TRUNCATE:
			truncarArchivo (fcbRecibido, tamanioNuevo);
			log_info (logger, "Truncar Archivo: <%s> - Tamaño: <%d>", fcbRecibido->nombre, tamanioNuevo);
			break;
		case F_READ:
		case F_WRITE:
			break;
	}
}