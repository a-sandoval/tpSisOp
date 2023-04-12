#include "../include/memoria.h"


int main(){

    t_config* memoria_config;
    logger = iniciarlogger("memoria.log", "Memoria");
	memoria_config = iniciarConfiguracion("memoria.config", logger);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, config_get_string_value(memoria_config,"PUERTO_ESCUCHA"));

	int rdoEjecucion = ejecutarServidor(cliente_fd,logger, obtenerCLavesValidas(memoria_config));

	if(rdoEjecucion == EXIT_FAILURE){
		printf("Terminando servidor");
		terminar_Programa(memoria_config,logger);
		return 0; 
	}

	terminar_Programa(memoria_config,logger);

	return 0;
    
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(t_config* memoriaconfig, t_log* logger)
{
	log_destroy(logger);
	config_destroy(memoriaconfig);
}

char* obtenerCLavesValidas(t_config memoria_config){

		
	ip = config_get_string_value(config, "IP_KERNEL");
}