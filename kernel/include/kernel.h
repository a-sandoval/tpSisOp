/* - Interprete entre el hardware y el software - */
#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"

//Por ser servidor
void iterator(char* value);


//Por ser cliente
// void leer_consola(t_log*); ?
void terminar_programa(char* , t_log*, t_config*); //Useless ahora mismo


#endif /* CLIENT_H_ */