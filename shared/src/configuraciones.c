#include "shared/include/configuraciones.h"

// CONFIG

t_config* iniciarConfiguracion(char* ruta){

    t_config* nuevoConfig = config_create(ruta); 
	
    if(nuevoConfig==NULL) {
		log_error(logger,"Error al generar archivo de config"); 
		log_destroy(logger); 
		exit(1); 
	}

    return nuevoConfig; 
}

char *confGet(char *key) {
	return config_get_string_value(config, key);
}

// LOGS 

t_log* iniciarLogger(char* nombreArchivo, char* nombreLogger){
	t_log* nuevoLogger = log_create(nombreArchivo, nombreLogger, 1, LOG_LEVEL_DEBUG);

	if(nuevoLogger == NULL) {
		perror("No se pudo crear el logger");
		exit(1);
	}

	return nuevoLogger;
}

t_log *cambiarNombre(t_log* logger, char *nuevoNombre) {
    t_log *nuevoLogger = logger;
	free(logger->program_name);
    nuevoLogger->program_name = strdup(nuevoNombre);
    return nuevoLogger;
}

char * duplicarNombre(t_log *logger) {
    return string_duplicate(logger->program_name);
}

void terminarPrograma() {
	log_destroy(logger);
	config_destroy(config);
}

FILE * abrir(int socket, char *archivo, char *tipoDeArchivo) {
    FILE *codigo = fopen(archivo, tipoDeArchivo);
    if (codigo == NULL) error(socket, "No se pudo abrir el archivo %s", archivo);
    return codigo;
}


void error (int socket, char *mensajeFormato, ...) {
    va_list argumentos;
    va_start(argumentos, mensajeFormato);

    char *mensajeCompleto = string_from_vformat(mensajeFormato, argumentos);

    log_error(logger, "%s", mensajeCompleto);

    va_end(argumentos);
    close(socket);
    terminarPrograma();
    exit(1);
}
