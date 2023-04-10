#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>
#include <readline/readline.h>
#include "../include/conexionMemoria.h"
#include "../../shared/include/utils.h"

int conexionMemoria(){

	/* Variables */

	int conexion;
	char* valor;
	char* ip;
	char* puerto;

	/*Configuraciones*/
	obtenerValoresConfig(&valor,&ip,&puerto);

	printf("VALOR: %s, IP: %s, PUERTO:%s",valor,ip,puerto);
	putchar('\n');

	/*Conexion con el servidor*/

	conexion = crear_conexion(ip, puerto);

	/*Si la memoria no esta activa, no puede realizarlo*/
	handshake(valor,conexion);

	/*Liberar memoria*/
	free(valor);
	free(ip);
	free(puerto);

	return conexion;

}

void obtenerValoresConfig(char**handshake_uso,char**ip_uso,char**puerto_uso){

	char* valor_handshake;
	char* ip;
	char* puerto;

	t_config* config;

	config = iniciar_config();

	valor_handshake = config_get_string_value(config,"CLAVE_CPU_MEMORIA");
	ip = config_get_string_value(config,"IP_MEMORIA");
	puerto = config_get_string_value(config,"PUERTO_MEMORIA");

	*handshake_uso=malloc(sizeof(char)*(strlen(valor_handshake) + 1));
	*ip_uso=malloc(sizeof(char)*(strlen(ip) + 1));
	*puerto_uso=malloc(sizeof(char)*(strlen(puerto) + 1));

	strcpy(*handshake_uso,valor_handshake);
	strcpy(*ip_uso,ip);
	strcpy(*puerto_uso,puerto);

	config_destroy(config);
}

void handshake(char*valor,int conexion){
	enviar_mensaje(valor, conexion);
}

t_config* iniciar_config(void){

	t_config* nuevo_config = config_create("cpu.config");

	if(nuevo_config == NULL){
		printf("No se pudo crear el config");
		exit(2);
	}

	return nuevo_config;
}
