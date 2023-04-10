#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utilsServidor.h"
#include "utilsCliente.h"

int alistarServidor();
int ejecutarServidor(int cliente_fd);
void iterator(char* value);
void element_destroyer(char*palabra);

#endif /* SERVER_H_ */
