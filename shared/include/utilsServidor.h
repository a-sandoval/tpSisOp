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
#include "manejoListas.h" 

void* recibir_buffer(int*, int);

int iniciar_servidor(char*puerto,t_log*logger);//Se le agrego un parametro
int esperar_cliente(int,t_log*);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
void element_destroyer(char*palabra);
extern void iterator(char *value);
int alistarServidor(t_log *logger, char *puerto);
int ejecutarServidor(int cliente_fd, t_log* logger, Lista* clavesValidas);
char* recibir_clave(int socket_cliente);

#endif /* UTILS_SERVIDOR_H */
