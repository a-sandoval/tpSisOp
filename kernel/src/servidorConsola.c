/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

// ESTO ESTA HARDCODEADISIMO
char* clavesValidas = {"usuario, admin"};
// Modificar cuando tenga funciones

int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola){
    char* puertoEscucha;
	obtenerDeConfiguracionComoServidor(&puertoEscucha, config_servidorConsola); 
	int socket_cliente = alistarServidor(logger_servidorConsola, puertoEscucha);
	int resultadoEjecucion = ejecutarServidor(socket_cliente, logger_servidorConsola,clavesValidas,1);
}

char* obtenerDeConfiguracionComoServidor(char ** puertoEscucha, t_config* config_servidorConsola) {
    *puertoEscucha = config_get_string_value(config_servidorConsola,"PUERTO_ESCUCHA");
    config_destroy(config_servidorConsola); 
    return puertoEscucha; 
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
