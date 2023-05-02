#ifndef UTILS_SERVIDOR_H
#define UTILS_SERVIDOR_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include <commons/config.h>
#include<string.h>
#include<assert.h>
#include"configuraciones.h"

extern int socketCliente;
extern t_log *logger;
extern t_config *config;

void* recibir_buffer(int* size, int socketCliente);
bool esClaveValida(void *clave);
int iniciarServidor(char*puerto);
int esperar_cliente(int);
t_list* recibir_paquete(int socketCliente);
int recibir_operacion(int socketCliente);
void element_destroyer(void* palabra);
void iterator(void *value);
int alistarServidor(char *puerto);
int ejecutarServidor(int socketCliente);

/*
void recibir_mensaje();
char* recibir_clave();

*/

#endif /* UTILS_SERVIDOR_H */ 