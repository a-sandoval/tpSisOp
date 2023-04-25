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
extern t_list *clavesValidas;

void* recibir_buffer(int*);
bool esClaveValida(void *clave);
int iniciar_servidor(char*puerto);
int esperar_cliente(int);
t_list* recibir_paquete();
void recibir_mensaje();
int recibir_operacion();
void element_destroyer(void* palabra);
extern void iterator(void *value);
void alistarServidor(char *puerto);
int ejecutarServidor();
char* recibir_clave();

#endif /* UTILS_SERVIDOR_H */ 
