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
		op_code cod_op = recibirOperacion(socketCliente); 

		int size, longDeNombre, desplazamiento = 0;
		char * nombreArchivo;
		fcb_t * nuevoArchivo, * fcbRecibido;
		void * data = recibirBuffer (socketCliente, &size);
		usleep (tiempoDeEspera * 1000);
		switch (cod_op) {
			case FOPEN:
				memcpy (&(longDeNombre), data + desplazamiento, sizeof (int)), desplazamiento += sizeof longDeNombre;
				nombreArchivo = malloc (sizeof (char) * longDeNombre);
				memcpy (nombreArchivo, data + desplazamiento, longDeNombre), desplazamiento += longDeNombre;
				log_info (logger, "Abrir Archivo: <%s>", nombreArchivo);
				nuevoArchivo = abrirArchivo (nombreArchivo);
				if (nuevoArchivo == NULL) enviarMensaje ("Apertura de archivo fallida, desea crear el archivo?", socketCliente);
				else enviarArchivo (nuevoArchivo, socketCliente);
				free (nombreArchivo), free (data);
				break;

			case FCREATE:
				memcpy (&(longDeNombre), data + desplazamiento, sizeof (int)), desplazamiento += sizeof longDeNombre;
				nombreArchivo = malloc (sizeof (char) * longDeNombre);
				memcpy (nombreArchivo, data + desplazamiento, longDeNombre), desplazamiento += longDeNombre;
				log_info (logger, "Crear Archivo: <%s>", nombreArchivo);
				if (crearArchivo(nombreArchivo) < 0) {
					enviarMensaje ("Fallo en la creacion de archivo :(", socketCliente);
					free (data), free (nombreArchivo);
					break;
				}
				nuevoArchivo = abrirArchivo (nombreArchivo);
				if (nuevoArchivo == NULL) enviarMensaje ("Fallo en la apertura de archivo :(", socketCliente);
				else enviarArchivo (nuevoArchivo, socketCliente);
				free (data), free (nombreArchivo);
				break;

			case FTRUNCATE:
				fcbRecibido = recibirArchivo (data, &(desplazamiento));
				desplazamiento += sizeof (int);
				uint32_t tamanioNuevo;
				memcpy (&(tamanioNuevo), data + desplazamiento, sizeof tamanioNuevo);
				log_info (logger, "Truncar Archivo: <%s> - Tamaño: <%d>", fcbRecibido->nombre, tamanioNuevo);
				if (truncarArchivo (fcbRecibido, tamanioNuevo) < 0) enviarMensaje ("Fallo truncacion de archivo :(", socketCliente);
				else enviarArchivo (fcbRecibido, socketCliente);
				free (data);
				break;

			case FREAD:
				fcbRecibido = recibirArchivo (data, &(desplazamiento));
				desplazamiento += sizeof (int);
				uint32_t puntero, tamanio, segmento;
				memcpy (& (puntero), data + desplazamiento, sizeof puntero);
				desplazamiento += sizeof (int) + sizeof puntero;
				memcpy (& (tamanio), data + desplazamiento, sizeof tamanio);
				desplazamiento += sizeof (int) + sizeof tamanio;
				memcpy (& (segmento), data + desplazamiento, sizeof segmento);
				char * leido = leerArchivo (fcbRecibido, puntero, tamanio);
				enviarAMemoria (leido, segmento, tamanio, conexionAMemoria);
				recibirOperacion (conexionAMemoria);
				char * mensaje = recibirMensaje (conexionAMemoria);
				if (!strcmp (mensaje, "Ejecucion correcta :)"))
					enviarMensaje("Yayay :)", socketCliente);
				free (mensaje), free (leido), free (fcbRecibido->nombre), free (fcbRecibido), free (data);
				break;

			case FWRITE:
				break;

			default:
				break;
		}
	}
}

fcb_t * recibirArchivo (void * data, int *desplazamiento) {
	fcb_t * fcbNuevo = malloc (sizeof (fcb_t));
	int longNombre;
	memcpy (&(longNombre), data + (* desplazamiento), sizeof longNombre);
	(* desplazamiento) += sizeof longNombre;
	fcbNuevo->nombre = malloc (sizeof (char) * longNombre);
	memcpy (fcbNuevo->nombre, data + (* desplazamiento), longNombre);
	(* desplazamiento) += longNombre + sizeof (int);
	memcpy (& (fcbNuevo->tamanio), data + (* desplazamiento), sizeof (uint32_t));
	(* desplazamiento) += sizeof (uint32_t) + sizeof (int);
	memcpy (& (fcbNuevo->ptrDirecto), data + (* desplazamiento), sizeof (uint32_t));
	(* desplazamiento) += sizeof (uint32_t) + sizeof (int);
	memcpy (& (fcbNuevo->ptrIndirecto), data + (* desplazamiento), sizeof (uint32_t));
	(* desplazamiento) += sizeof (uint32_t);
	return fcbNuevo;
	
}

int enviarArchivo (fcb_t * archivo, int socket) {
	t_paquete * paquete = crearPaquete ();
	agregarAPaquete (paquete, &(archivo->tamanio), sizeof archivo->tamanio);
	agregarAPaquete (paquete, &(archivo->ptrDirecto), sizeof archivo->ptrDirecto);
	agregarAPaquete (paquete, &(archivo->ptrIndirecto), sizeof archivo->ptrIndirecto);
	enviarPaquete (paquete, socket);
	free (paquete->buffer), free (paquete), free (archivo->nombre), free (archivo);
	return 0;
}

int enviarAMemoria (char * mensaje, uint32_t segmento, uint32_t tamanio, int socket) {
	t_paquete * paquete = crearPaquete ();
	paquete->codigo_operacion = segmento;
	agregarAPaquete (paquete, (void *) mensaje, tamanio);
	enviarPaquete (paquete, socket);
	free (paquete->buffer), free (paquete);
	return 0;
}