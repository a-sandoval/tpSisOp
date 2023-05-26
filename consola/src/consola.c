#include "consola/include/consola.h"

int socketCliente;
t_log *logger;
t_config *config;

int main(int, char *archivos[]) {
    logger = iniciarLogger("consola.log", "consola");
    config = iniciarConfiguracion(archivos[2]);
    socketCliente = -1;
    while(!(socketCliente + 1)) {
        socketCliente = conexion("KERNEL");
        if (!(socketCliente + 1)) {
            log_warning(logger, "No se pudo conectar a la Kernel, esperando 5 segundos.");
            sleep(5);
        }
    }
    FILE *codigo = abrir(archivos[1], "r");
    size_t cantChars = MAX_CHARS;

    while(!feof(codigo)) {
        char *linea = (char *) malloc (sizeof (char) * cantChars);
        if (linea == NULL) error("No se pudo alocar memoria");

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
    esperarFinalizacion();

    close(socketCliente);
    fclose(codigo);
    terminarPrograma();

    return 0;
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