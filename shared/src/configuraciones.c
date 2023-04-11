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


// LOGS 


t_log* iniciar_logger(char* nombreArchivo, char* nombreModulo)
{
	t_log* nuevo_logger = log_create(nombreArchivo, nombreModulo,1,LOG_LEVEL_DEBUG);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger");
		exit(1);
	}

	return nuevo_logger;
}



