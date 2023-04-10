#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "../../shared/include/utilsServidor.h"

int alistarServidor();
int ejecutarServidor(int cliente_fd);
void iterator(char* value);
void element_destroyer(char*palabra);
void obtenerPuertoEscucha(char**puerto_escucha);

#endif /* SERVER_H_ */
