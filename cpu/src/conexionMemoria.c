
#include "../include/conexionMemoria.h"


int conexionMemoria(t_config* config){

	/* Variables */

	int conexion;
	char* valor = config_get_string_value(config,"CLAVE_CPU_MEMORIA");
	char* ip = config_get_string_value(config,"IP_MEMORIA");
	char* puerto = config_get_string_value(config,"PUERTO_MEMORIA");

	/*Configuraciones*/
	printf("VALOR: %s, IP: %s, PUERTO:%s",valor,ip,puerto);
	putchar('\n');

	conexion = realizarConexion(ip,puerto,valor);

	/*Liberar memoria*/
	free(valor);
	free(ip);
	free(puerto);

	return conexion;

}
