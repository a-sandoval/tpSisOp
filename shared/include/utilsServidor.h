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

extern t_log *logger;
extern t_config *config;
extern t_list *clavesValidas;

void* recibir_buffer(int*, int);

bool esClaveValida(void *clave);

int iniciar_servidor(char*puerto,t_log*logger);
int esperar_cliente(int,t_log*);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
void element_destroyer(void* palabra);
extern void iterator(char *value);
int alistarServidor(char *puerto);
int ejecutarServidor(int cliente_fd);
char* recibir_clave(int socket_cliente);

#endif /* UTILS_SERVIDOR_H */ 
