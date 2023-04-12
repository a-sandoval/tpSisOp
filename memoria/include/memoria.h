#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<string.h>
#include "../include/utils.h"
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<assert.h>

void iterator(char* value);

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

extern t_log* logger;

#endif 

