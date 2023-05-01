#include "consola/include/consola.h"

int main(int, char *archivos[]) {

    // inicializacion de las variables
    logger = iniciarLogger("consola.log", "consola");

    config = iniciarConfiguracion(archivos[2]);

    /*socketClienteFD = conexion("KERNEL");
    if (!(socketClienteFD + 1)) { 
        log_error(logger, "No se pudo conectar a la Kernel");
        log_destroy(logger);
        config_destroy(config);
        return 1;
    }
    */
    // apertura del archivo de pseudocodigo

    FILE *codigo = fopen(archivos[1], "r");

    if (codigo == NULL) {
        log_error(logger, "No se pudo abrir el archivo %s", archivos[1]);
        close(socketCliente);
        terminarPrograma(NULL);
        return 1;
    }

    // leer el archivo y enviar paquetes del codigo

    while(!feof(codigo)) {
        char *lineaCodigo = malloc(sizeof(char));
        int i = 0;
        for (; ((lineaCodigo[i] = fgetc(codigo) )!= '\n') && !feof(codigo); i++);
        lineaCodigo[i] = '\0';
        printf("%s \n", lineaCodigo);

        t_paquete *paquete = crearPaquete();
        agregar_a_paquete (paquete, lineaCodigo, sizeof(char) * strlen(lineaCodigo) + 1);

        //enviar_paquete(paquete, socketClienteFD);
        eliminar_paquete(paquete);
        free(lineaCodigo);
        //sleep(1);
    }

    // cerrar archivos

    close(socketCliente);
    fclose(codigo);
    terminarPrograma(NULL);

    return 0;
}
