#include "../include/configuraciones.h"

//ARCHIVO DE CONFIGURACION (CLIENTE)
t_config* iniciarConfiguracion(char* ruta, t_log* logger) {

    t_config* nuevo_config=config_create(ruta); 

    if(nuevo_config==NULL) {
		log_error(logger,"Error al generar archivo de config"); 
		log_destroy(logger); 
		exit(1); 
	}

    return nuevo_config; 
}


// LOGS 

t_log* iniciar_logger(char* nombreArchivo, char* nombreLogger)
{
	t_log* nuevo_logger = log_create(nombreArchivo, nombreLogger,1,LOG_LEVEL_DEBUG);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger");
		exit(2);
	}

	return nuevo_logger;
}



