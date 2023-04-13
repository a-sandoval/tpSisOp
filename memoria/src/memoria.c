#include "../include/memoria.h"



int main(){

    t_config* memoria_config;
    logger = iniciar_logger("memoria.log", "Memoria");
	memoria_config = iniciarConfiguracion("memoria.config", logger);

	Lista* clavesValidas = malloc(sizeof(Lista));
	clavesValidas->cabeza=NULL;
	//obtenerClavesValidas(memoria_config); ORIGINAL
	obtenerClavesValidas(memoria_config,clavesValidas);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, config_get_string_value(memoria_config,"PUERTO_ESCUCHA"));

	int rdoEjecucion = ejecutarServidor(cliente_fd,logger, clavesValidas);

	if(rdoEjecucion == EXIT_FAILURE){
		printf("Terminando servidor");
		terminar_programa(memoria_config,logger);
		borrarLista(clavesValidas);
		return 0; 
	}

	terminar_programa(memoria_config,logger);
	borrarLista(clavesValidas);

	return 0;
    
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(t_config* memoriaconfig, t_log* logger){
	log_destroy(logger);
	config_destroy(memoriaconfig);
}


void obtenerClavesValidas(t_config* memoria_config,Lista* claves){
	
	insertar(claves, config_get_string_value(memoria_config, "CLAVE_CPU_MEMORIA"));
	insertar(claves, config_get_string_value(memoria_config, "CLAVE_KERNEL_MEMORIA"));
	insertar(claves, config_get_string_value(memoria_config, "CLAVE_FS_MEMORIA"));
}
