#include "fileSystem/include/servidorKernel.h"

int tiempoDeEspera;

void escucharAlKernel() {
    char* puertoEscucha = confGet ("PUERTO_ESCUCHA");
    socketCliente = alistarServidor (puertoEscucha);
	log_info(logger, "Se conecto el Kernel");
    ejecutarServidor();  
}

void ejecutarServidor() {
	tiempoDeEspera = config_get_int_value (config, "RETARDO_ACCESO_BLOQUE");
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
	int size, desplazamiento = sizeof (int);
	void * data = recibirBuffer (socketCliente, &size);
	operacionFS_e instruccion;
	memcpy (&(instruccion), data + desplazamiento, sizeof instruccion), desplazamiento += sizeof instruccion;
	
	//fcb_t * fcbRecibido = malloc (sizeof (fcb_t));
	usleep (tiempoDeEspera * 1000);
	switch (instruccion) {
		case F_OPEN:
			int longDeNombre;
			memcpy (&(longDeNombre), data + desplazamiento, sizeof (int)), desplazamiento += sizeof longDeNombre;
			char *nombreArchivo = malloc (sizeof (char) * longDeNombre);
			fcb_t * nuevoArchivo = abrirArchivo (nombreArchivo);
			if (nuevoArchivo == NULL) {
				enviarMensaje ("Apertura de archivo fallida, desea crear el archivo?", socketCliente);
				char * respuesta = recibirMensaje (socketCliente);
				if (!strcmp (respuesta, "si")) {
					int retCode = crearArchivo (nombreArchivo);
					if (retCode < 0) 
						enviarMensaje ("Fallo en la creación de archivo", socketCliente);
					nuevoArchivo = abrirArchivo (nombreArchivo);
					if (nuevoArchivo == NULL)
						enviarMensaje ("Fallo en la apertura de archivo :(", socketCliente);
				}
				else {
					free (nombreArchivo), free (data), free (respuesta);
					break;
				}
			}
			log_info (logger, "Abrir Archivo: <%s>", nombreArchivo);
			enviarArchivo (nuevoArchivo);
			free (nombreArchivo), free (data);
		case F_CREATE:
			if (crearArchivo(nombreArchivo) < 0)
				enviarMensaje ("Fallo :(", socketCliente);
			log_info (logger, "Crear Archivo: <%s>", nombreArchivo);
			nuevoArchivo = abrirArchivo (nombreArchivo);
			break;
		case F_TRUNCATE:
			fcb_t * fcbRecibido = recibirFCB (data, &(desplazamiento), socketCliente);
			desplazamiento += sizeof (int);
			uint32_t tamanioNuevo = (uint32_t) memcpy (&(tamanioNuevo), data + desplazamiento, sizeof tamanioNuevo);
			int retCode = truncarArchivo (fcbRecibido, tamanioNuevo);
			if (retCode < 0) {
				enviarMensaje ("Fallo truncacion de archivo :(", socketCliente);
				break;
			} else {
				enviarArchivo (fcbRecibido);
			}
			log_info (logger, "Truncar Archivo: <%s> - Tamaño: <%d>", fcbRecibido->nombre, tamanioNuevo);
			free (fcbRecibido->nombre), free (data), free (fcbRecibido);
			break;
		case F_READ:
			fcb_t * fcbRecibido = recibirArchivo (data, &(desplazamiento), socketCliente);
			desplazamiento += sizeof (int);
			uint32_t puntero = memcpy (& (puntero), data + desplazamiento, sizeof puntero);
			desplazamiento += sizeof (int) + sizeof puntero;
			uint32_t tamanio = memcpy (& (tamanio), data + desplazamiento, sizeof tamanio);
			desplazamiento += sizeof (int) + sizeof tamanio;
			uint32_t segmento = memcpy (& (segmento), data + desplazamiento, sizeof segmento);
			char * leido = leerArchivo (fcbRecibido, puntero, tamanio);
			enviarAMemoria (leido, conexionAMemoria);
			recibirOperacion (conexionAMemoria);
			char * mensaje = recibirMensaje (conexionAMemoria);
			if (!strcmp (mensaje, "Ejecucion correcta :)"))
				enviarMensaje("Yayay :)", socketCliente);
			free (mensaje), free (leido), free (fcbRecibido->nombre), free (fcbRecibido), free (data);
			break;
		case F_WRITE:
			break;
	}
}

fcb_t * recibirArchivo (void * data, int *desplazamiento, int socket) {
	fcb_t * fcbNuevo = malloc (sizeof (fcb_t));
	int longNombre = memcpy (&(longNombre), data + (* desplazamiento), sizeof longNombre);
	(* desplazamiento) += sizeof longNombre;
	fcbNuevo->nombre = malloc (sizeof (char) * longNombre);
	fcbNuevo->nombre = memcpy (fcbNuevo->nombre, data + (* desplazamiento), longNombre);
	(* desplazamiento) += longNombre + sizeof (int);
	fcbNuevo->tamanio = memcpy (& (fcbNuevo->tamanio), data + (* desplazamiento), sizeof (uint32_t));
	(* desplazamiento) += sizeof (uint32_t) + sizeof (int);
	fcbNuevo->ptrDirecto = memcpy (& (fcbNuevo->ptrDirecto), data + (* desplazamiento), sizeof (uint32_t));
	(* desplazamiento) += sizeof (uint32_t) + sizeof (int);
	fcbNuevo->ptrIndirecto = memcpy (& (fcbNuevo->ptrIndirecto), data + (* desplazamiento), sizeof (uint32_t));
	(* desplazamiento) += sizeof (uint32_t);
	return fcbNuevo;
	
}