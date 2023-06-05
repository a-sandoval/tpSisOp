#include "consola/include/consola.h"

int socketCliente;
t_log *logger;
t_log *loggerError;
t_config *config;

int main(int, char *archivos[]) {
    logger = iniciarLogger("consola.log", "consola");
    loggerError = iniciarLogger("consolaErrores.log", "consolaErrores");
    config = iniciarConfiguracion(archivos[2]);
    atexit(terminarPrograma);
    socketCliente = -1;
    while(!(socketCliente + 1)) {
        socketCliente = conexion("KERNEL");
        if (!(socketCliente + 1)) {
            log_warning(logger, "No se pudo conectar a la Kernel, esperando 5 segundos.");
            sleep(5);
        }
    }
    FILE *codigo = abrir(socketCliente, archivos[1], "r");
    size_t cantChars = MAX_CHARS;

    while(!feof(codigo)) {
        char *linea = (char *) malloc (sizeof (char) * cantChars);
        if (linea == NULL) error(socketCliente, "No se pudo alocar memoria");

        getline(&linea, &cantChars, codigo);
        char *lineaCodigo = string_replace(linea, "\n\0", "\0");

        t_paquete *paquete = crearPaquete();
        agregarAPaquete ( paquete, lineaCodigo, sizeof(char) * strlen(lineaCodigo) + 1 );
        enviarPaquete(paquete, socketCliente);
        eliminarPaquete(paquete);
        free(lineaCodigo);
        free(linea);
    }
    enviarMensaje("Fin de instrucciones", socketCliente);
    esperarPID(archivos[1]);
    esperarFinalizacion();

    close(socketCliente);
    fclose(codigo);

    exit(0);
}

void esperarPID(char * archivo) {
	int cod_op;
	recv(socketCliente, &cod_op, sizeof(int), MSG_WAITALL);
	int size;
	recv(socketCliente, &size, sizeof(int), MSG_WAITALL);
    char * pidString = malloc(size * sizeof (char));
	recv(socketCliente, pidString, size, MSG_WAITALL);
    logger = cambiarNombre(logger, string_from_format("Consola - <%s> - <%s>", archivo, pidString));
    return;

}

void esperarFinalizacion () {
	int cod_op;
	recv(socketCliente, &cod_op, sizeof(int), MSG_WAITALL);
	int size;
	recv(socketCliente, &size, sizeof(int), MSG_WAITALL);
	char * mensaje = malloc(size);
	recv(socketCliente, mensaje, size, MSG_WAITALL);
    log_info(logger, mensaje);
    free(mensaje);
    return;
}