#include "../include/memoria.h"


int main(){

    t_config* memoriaconfig;
    logger = iniciarlogger("memoria.log", "Memoria");
	memoriaconfig = iniciarConfiguracion("memoria.config", logger);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, config_get_string_value(memoriaconfig,"PUERTO_ESCUCHA"));

	int rdoEjecucion = ejecutarServidor(cliente_fd,logger);

	if(rdoEjecucion == EXIT_FAILURE){
		printf("Terminando servidor");
		terminar_Programa(memoriaconfig,logger);
		return 0; 
	}
	

	terminar_Programa(memoriaconfig,logger);

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
