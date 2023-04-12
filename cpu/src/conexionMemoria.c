
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

	/*Conexion con el servidor*/

	conexion = crear_conexion(ip, puerto);
	if(conexion == -1){
		return -1;
	}

	/*Si la memoria no esta activa, no puede realizarlo*/
	handshake(valor,conexion);

	/*Liberar memoria*/
	free(valor);
	free(ip);
	free(puerto);

	return conexion;

}


void handshake(char*valor,int conexion){
	enviar_mensaje(valor, conexion);
}
