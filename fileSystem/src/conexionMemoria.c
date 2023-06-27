#include "fileSystem/include/conexionMemoria.h"

int conexionAMemoria; 

void conexionMemoria() {

    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre(logger, "File System - Memoria");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre(loggerError,"Errores File System - Memoria");
    conexionAMemoria = conexion("MEMORIA");
    logger = cambiarNombre(logger, nombreAnterior);
    loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);
    free (nombreAnterior), free (nombreAnteriorErrores);
    return;
}

int enviarAMemoria (char * mensaje, uint32_t segmento, uint32_t tamanio, int socket) {
	t_paquete * paquete = crearPaquete ();
	paquete->codigo_operacion = WRITE;
    agregarAPaquete (paquete, &(segmento), sizeof segmento);
	agregarAPaquete (paquete, (void *) mensaje, tamanio);
	enviarPaquete (paquete, socket);
	free (paquete->buffer), free (paquete);
	return 0;
}

char * solicitarAMemoria (uint32_t segmento, uint32_t tamanio, int socket) {
    t_paquete * paquete = crearPaquete ();
	paquete->codigo_operacion = READ;
	agregarAPaquete (paquete, &(segmento), sizeof segmento);
	agregarAPaquete (paquete, &(tamanio), sizeof tamanio);
	enviarPaquete (paquete, socket);
	free (paquete->buffer), free (paquete);
    recibirOperacion (socket);
    char * data = recibirMensaje (socket);
    return data;
}