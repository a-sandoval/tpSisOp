#include "consola/include/consola.h"

int main(int num_args, char *argumentos[]) {
    int conexion;
    char *ip, *puerto;
    rl_attempted_completion_function = autoCompletar;
    t_log* logger = log_create("consola.log", "consola", 0, LOG_LEVEL_INFO);
    if (logger == NULL) {
        printf("Error al generar logger consola.log \n");
        return 1;
    }

    t_config* config = config_create("consola.config");
    if (config == NULL) {
        log_error(logger, "Error al abrir consola.config");
        log_destroy(logger);
        return 1;
    }

    ip = config_get_string_value(config, "IP_KERNEL");
    puerto = config_get_string_value(config, "PUERTO_KERNEL");
    conexion = crear_conexion(ip, puerto);

    if (conexion == -1) { 
        char *mensaje_error = string_from_format("No se pudo conectar a la Kernel por la ip %s y puerto %s", ip, puerto);
        log_error(logger, mensaje_error);
        log_destroy(logger);
        config_destroy(config);
        free(mensaje_error);
        return 1;
    }

    while (0) {
        leer_consola(logger, conexion);
    }
    
    log_destroy(logger);
    config_destroy(config);

    return 0;
}

void leer_consola(t_log *logger, int conexion) {
    char *comando = readline("Usuario@TUKI $ ");
    string_trim(&comando);
    if (!strcmp(comando, "EXIT")) {free(comando); break;}
    if (*comando) {
        enviar_mensaje(comando, conexion);
        add_history(comando); 
    }
    log_info(logger, comando);
    free(comando);
}

char **autoCompletar(const char *texto, int inicio, int final) {
    char **lista = NULL;
    if (!inicio) {
        lista = rl_completion_matches(texto, comandoPosible);
    }
    return lista;
}

char *comandoPosible(const char *texto, int estado) {
    int i, len;
    char *nombre;
    if (!estado) {
        i = 0;
        len = strlen(texto);
    }
    while (nombre = listaComandos[i]) {
        i++;
        if (!strncmp(nombre, texto, len)) return string_duplicate(nombre);
    }
    return NULL;
}

