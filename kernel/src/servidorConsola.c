/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola){
	Lista* clavesValidas = NULL;
	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger_servidorConsola, config_get_string_value(config_servidorConsola,"PUERTO_ESCUCHA"));

	int rdoEjecucion = ejecutarServidor(cliente_fd,logger_servidorConsola, clavesValidas);

	if(rdoEjecucion == EXIT_FAILURE){
		printf("Terminando servidor");
		terminar_programa_sv(config_servidorConsola, logger_servidorConsola);
		borrarLista(clavesValidas);
		return 0; 
	}

	terminar_programa_sv(config_servidorConsola,logger_servidorConsola);
	borrarLista(clavesValidas);

	return 0;
}

void terminar_programa_sv(t_config* config_servidorConsola, t_log* logger_servidorConsola){
	log_destroy(logger_servidorConsola);
	config_destroy(config_servidorConsola);
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

Lista* obtenerClavesValidas(t_config* config_servidorConsola){
	Lista* claves = NULL;
	insertar(claves, config_get_string_value(config_servidorConsola, "CLAVE_CONSOLA_KERNEL"));
	return claves;
}