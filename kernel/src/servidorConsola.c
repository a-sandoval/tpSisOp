/* CONSOLA - cliente | KERNEL - sevidor*/
#include "kernel/include/servidorConsola.h"

int servirAConsola(t_log* logger_servidorConsola, t_config* config_servidorConsola){
    /*Preparacion del kernel para servir a la consola*/
	int conexion_servir_consola = alistarServidor(logger_servidorConsola,config_get_string_value(config_servidorConsola,"PUERTO_ESCUCHA"));

	//Todo esto debería estar en una función
	int principal = ejecutarServidorTrucho(conexion_servir_consola,logger_servidorConsola);
    if(principal == EXIT_FAILURE){
		printf("Terminando servidor");
		terminar_Programa(config_servidorConsola,logger_servidorConsola);
		return 0; 
	}

    terminar_Programa(config_servidorConsola,logger_servidorConsola);
	return 0;
}

