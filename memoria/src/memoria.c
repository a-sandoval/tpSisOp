#include "../include/memoria.h"


int main(){

    t_config* memoriaconfig;
    logger = iniciarlogger("memoria.log", "Memoria");
	memoriaconfig = iniciarConfiguracion("memoria.config", logger);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, config_get_string_value(memoriaconfig,"PUERTO_ESCUCHA"));

	int  = ejecutarServidor(cliente_fd,logger);
	
	terminar_Programa(memoriaconfig,logger);
    


}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(t_config* memoriaconfig, t_log* logger)
{
	log_destroy(logger);
	config_destroy(memoriaconfig);
}
