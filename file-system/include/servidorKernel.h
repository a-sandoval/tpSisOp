#ifndef FILE_SYS_KERN_SER_H
#define FILE_SYS_KERN_SER_H

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/config.h>
#include <stdbool.h>

#include "utilsCliente.h" 
#include "utilsServidor.h" 


#endif

t_log* logger; 

void escucharAlKernel(); 
char* obtenerDeConfiguracionComoServidor(char**);
