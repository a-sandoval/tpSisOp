#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<readline/readline.h>
#include<readline/history.h>

#include "shared/include/utilsCliente.h"
#include "shared/include/configuraciones.h"

typedef struct {
    char *nombre;
    int  cantParametros;
} t_comando;

t_comando listaComandos[] = {
    {"F_READ", 3}, {"F_WRITE", 3}, 
    {"CREATE_SEGMENT", 2}, {"F_SEEK", 2}, {"F_TRUNCATE", 2}, {"MOV_IN", 2}, {"MOV_OUT", 2}, {"SET", 2}, 
    {"DELETE_SEGMENT", 1}, {"F_CLOSE", 1}, {"F_OPEN", 1}, {"I/O", 1}, {"SIGNAL", 1}, {"WAIT", 1}, 
    {"EXIT", 0}, {"YIELD", 0}, 
    {(char *)NULL, -1}
};

typedef struct {
    int  longNombre;
    int  cantParametros;
    int  longParametros[3];
    char *nombre;
    t_queue *filaParametros;
} t_comando_total;

t_comando buscarComando(char *comando);

t_comando_total prepararComando(t_comando comando, char **parametros);

#endif /* CLIENT_H_ */