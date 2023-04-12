/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola){
    char* puertoEscucha;
	obtenerDeConfiguracionComoServidor(&puertoEscucha, config_servidorConsola); 
	int servir_a_consola = alistarServidor(logger_servidorConsola, puertoEscucha);
	int resultadoEjecucion = ejecutarServidorTrucho(servir_a_consola, logger_servidorConsola);  
}

char* obtenerDeConfiguracionComoServidor(char ** puertoEscucha, t_config* config_servidorConsola) {
    *puertoEscucha = config_get_string_value(config_servidorConsola,"PUERTO_ESCUCHA");
    config_destroy(config_servidorConsola); 
    return puertoEscucha; 
}
