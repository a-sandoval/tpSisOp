#include "consola/include/consola.h"

int main(int, char *archivos[]) {
    logger        = iniciarLogger("consola.log", "consola");
    config        = iniciarConfiguracion(archivos[2]);
    socketCliente = conexion("KERNEL");
    if (!(socketCliente + 1)) error("No se pudo conectar a la Kernel");
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