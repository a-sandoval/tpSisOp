#include "consola/include/consola.h"

int main(void) {
    int conexion;
    char *ip, *puerto;
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
        return 1;
    }

    char *comando = readline("Usuario@TUKI $ ");
    log_info(logger, comando);
    string_trim(&comando);
    while (strcmp(comando, "EXIT")) {
        free(comando);
        comando = readline("Usuario@TUKI $ ");
        log_info(logger, comando);
        string_trim(&comando);
    }
    
    free(comando);
    log_destroy(logger);
    config_destroy(config);

    return 0;
}

