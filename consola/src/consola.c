#include "consola/include/consola.h"

int main(int, char *archivos[]) {

    // inicializacion de las variables

    int conexion;
    char *ip, *puerto;
    t_log* logger = iniciarLogger("consola.log", "consola");

    t_config* config = iniciarConfiguracion(archivos[2], logger);
    ip = config_get_string_value(config, "IP_KERNEL");
    puerto = config_get_string_value(config, "PUERTO_KERNEL");

    // conexion al kernel

    conexion = crear_conexion(ip, puerto);

    if (conexion == -1) { 
        char *mensaje_error = string_from_format("No se pudo conectar a la Kernel por la ip %s y puerto %s", ip, puerto);
        log_error(logger, mensaje_error);
        free(mensaje_error);
        log_destroy(logger);
        config_destroy(config);
        return 1;
    }
    
    // apertura del archivo de pseudocodigo

    FILE *codigo = fopen(archivos[1], "r");

    if (codigo == NULL) {
        char *mensaje_error = string_from_format("No se pudo abrir el archivo %s", archivos[1]);
        log_error(logger, mensaje_error);
        free(mensaje_error);
        log_destroy(logger);
        config_destroy(config);
        close(conexion);
        return 1;
    }
    //rl_attempted_completion_function = autoCompletar;
    //leer_consola(logger, conexion);

    //string_array_new y string_replace dan leaks

    // leer el archivo y enviar paquetes del codigo

    char lineaCodigo[1024];
    while(!feof(codigo)) {
        fgets(lineaCodigo, sizeof(lineaCodigo), codigo);
        char **listaParametros = string_n_split(lineaCodigo, 4, " ");
        t_comando comando = buscarComando(listaParametros[0]);

        if (comando.nombre != NULL) {
            char *temp = string_array_pop(listaParametros);
            string_array_push(&listaParametros, string_substring_until(temp, strlen(temp) - 1));
            free(temp);
            
            t_comando_total comComp = prepararComando(comando, listaParametros);

            free(comComp.nombre);
            queue_destroy_and_destroy_elements(comComp.filaParametros, (void *)free);
        }
        string_array_destroy(listaParametros);
    }

    // cerrar archivos

    fclose(codigo);
    log_destroy(logger);
    config_destroy(config);

    return 0;
}

t_comando buscarComando(char *comando) {
    int i = 0;
    while(listaComandos->cantParametros != -1 && !string_contains(comando, listaComandos[i].nombre)) i++;
    return listaComandos[i];
}

t_comando_total prepararComando(t_comando comando, char **parametros) {
    printf("%s ", comando.nombre);
    for (int i = 1; i <= comando.cantParametros; i++) 
        printf("\"%s\" ", parametros[i]);
    printf("\n");   

    t_comando_total comandoCompleto;
    comandoCompleto.cantParametros = comando.cantParametros;
    comandoCompleto.filaParametros = queue_create();
    for (int i = 1; i < comando.cantParametros; i++) {
        comandoCompleto.longParametros[i - 1] = (i < comando.cantParametros) ? strlen(parametros[i]) : 0;
        queue_push(comandoCompleto.filaParametros, (void *)comando.nombre);
        //string_array_push(&(comandoCompleto.parametros), parametros[i]);
    }
    comandoCompleto.longNombre = strlen(comando.nombre);
    comandoCompleto.nombre = strdup(comando.nombre);

    return comandoCompleto;
}
