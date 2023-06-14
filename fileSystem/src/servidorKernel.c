#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {
    char* puertoEscucha = confGet ("PUERTO_ESCUCHA");
    socketCliente = alistarServidor (puertoEscucha);
	log_info(logger, "Se conecto el Kernel");
    ejecutarServidor();  
}

void ejecutarServidor() {
	while (1) {
		int cod_op = recibirOperacion(socketCliente); 
		switch (cod_op) {
			case PAQUETE:
				recibirInstruccion (socketCliente);
				break;
			case MENSAJE:
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

void recibirInstruccion (int socket) {
	int size, desplazamiento = 0;
	void *data = recibirBuffer (socketCliente, &size);
	desplazamiento += sizeof (int);
	operacionFS_e instruccion;
	memcpy (&(instruccion), data + desplazamiento, sizeof (operacionFS_e)), desplazamiento += sizeof desplazamiento;
	
	//fcb_t * fcbRecibido = malloc (sizeof (fcb_t));
	usleep (tiempoDeEspera * 1000);
	switch (instruccion) {
		case F_OPEN:
			int longDeNombre;
			memcpy (&(longDeNombre), data + desplazamiento, sizeof (int)), desplazamiento += sizeof longDeNombre;
			char *nombreArchivo = malloc (sizeof (char) * longDeNombre);
			fcb_t * nuevoArchivo = abrirArchivo (nombreArchivo);
			if (nuevoArchivo == NULL) {
			}
			log_info (logger, "Abrir Archivo: <%s>", nombreArchivo);
			enviarArchivo ();
			free (nombreArchivo);
		case F_CREATE:
			if (crearArchivo(nombreArchivo) < 0)
				enviarMensaje ("Fallo :(", socketCliente);
			log_info (logger, "Crear Archivo: <%s>", nombreArchivo);
			nuevoArchivo = abrirArchivo (nombreArchivo);
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