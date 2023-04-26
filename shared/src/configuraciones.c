#include "shared/include/configuraciones.h"

//ARCHIVO DE CONFIGURACION (CLIENTE)
t_config* iniciarConfiguracion(char* ruta){

    t_config* nuevoConfig=config_create(ruta); 

    if(nuevoConfig==NULL) {
		log_error(logger,"Error al generar archivo de config"); 
		log_destroy(logger); 
		exit(1); 
	}

    return nuevoConfig; 
}

// LOGS 

t_log* iniciarLogger(char* nombreArchivo, char* nombreLogger){
	t_log* nuevoLogger = log_create(nombreArchivo, nombreLogger, 1, LOG_LEVEL_DEBUG);

	if(nuevoLogger == NULL){
		printf("No se pudo crear el logger");
		exit(2);
	}

	return nuevoLogger;
}

t_log *cambiarNombre(char *nuevoNombre) {
    t_log *nuevoLogger = logger;
	free(logger->program_name);
    nuevoLogger->program_name = strdup(nuevoNombre);
    return nuevoLogger;
}

char *confGet(char *key) {
	return config_get_string_value(config, key);
}

void terminarPrograma()
{
	log_destroy(logger);
	config_destroy(config);

}