/* CONSOLA - cliente | KERNEL - sevidor*/

#include "kernel/include/servidorConsola.h"



void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(t_config* config_servidorConsola, t_log* logger_ServidorConsola){
	log_destroy(logger_ServidorConsola);
	config_destroy(config_servidorConsola);
}