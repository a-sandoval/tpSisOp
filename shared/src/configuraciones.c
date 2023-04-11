#include "configuraciones.h"

//ARCHIVO DE CONFIGURACION cliente

t_config* iniciarConfiguracion(char* ruta, t_log* logger) {

    t_config* nuevo_config=config_create(ruta); 

    if(nuevo_config==NULL) {
		log_error(logger,"Error al generar archivo de config"); 
		log_destroy(logger); 
		return NULL; 
	}

    return nuevo_config; 
}

char* obtenerStringDe(t_config* configuracion, char* key) {
    return config_get_string_value(configuracion,key); 
}


// LOGS 


t_log* iniciar_logger(char* nombreArchivo, char* nombreModulo)
{
	t_log* nuevo_logger = log_create(nombreArchivo, nombreModulo,1,LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger");
		exit(1);
	}

	return nuevo_logger;
}



