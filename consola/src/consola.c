#include "consola/include/consola.h"

int recibir_operacion(){
	int cod_op;
	if(recv(socketCliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socketCliente);
		return -1;
	}
}

void* recibir_buffer(int* size){
	void * buffer;
	recv(socketCliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socketCliente, buffer, *size, MSG_WAITALL);
	return buffer;
}

char *recibirMensaje()
{
	int size;
	char *buffer = recibir_buffer(&size);
	return buffer;
}

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
    recibir_operacion();
    char *buffer = recibirMensaje();
    log_info(logger, buffer);
    free(buffer);

    close(socketCliente);
    fclose(codigo);
    terminarPrograma();

    return 0;
}

FILE *abrir(char *archivo, char *tipoDeArchivo) {
    FILE *codigo = fopen(archivo, tipoDeArchivo);
    if (codigo == NULL) error("No se pudo abrir el archivo %s", archivo);
    return codigo;
}

void error (char *mensajeFormato, ...) {
    va_list argumentos;
    va_start(argumentos, mensajeFormato);

    char *mensajeCompleto = string_from_vformat(mensajeFormato, argumentos);

    log_error(logger, "%s", mensajeCompleto);

    va_end(argumentos);
    close(socketCliente);
    terminarPrograma();
    exit(1);
}