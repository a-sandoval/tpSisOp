/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola){

	char* puertoDeEscucha = config_get_string_value(config_servidorConsola,"PUERTO_ESCUCHA"); 
	
	Lista* clavesValidas = malloc(sizeof(Lista));
	clavesValidas->cabeza = NULL;

	obtenerClavesValidas(config_servidorConsola,clavesValidas);
	
	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger_servidorConsola, puertoDeEscucha);

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd,logger_servidorConsola, clavesValidas);

	log_info(logger_servidorConsola,"Terminando servidor");

	borrarLista(clavesValidas);

	return 0;
}

void iterator(char *value) {
    t_log *logger_servidorConsolaTemp = iniciar_logger("KERNELservidor.log", "Consola-Kernel");
    log_info(logger_servidorConsolaTemp, value);
    log_destroy(logger_servidorConsolaTemp);
}

void obtenerClavesValidas(t_config* config_servidorConsola,Lista* claves){
	insertar(claves, config_get_string_value(config_servidorConsola, "CLAVE_CONSOLA_KERNEL_MODO_USUARIO"));
	insertar(claves, config_get_string_value(config_servidorConsola, "CLAVE_CONSOLA_KERNEL_MODO_ADMIN"));
}