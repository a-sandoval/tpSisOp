#ifndef FILE_SYS_MEMH
#define FILE_SYS_MEM_H

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/config.h>

#include "shared/include/utilsCliente.h" 
#include "shared/include/utilsServidor.h" 

t_log* logger; 

void conectarseAMemoria(); 

void obtenerDeConfiguracionComoCliente(char** puerto, char** ip, char** claveHandshake); 


#endif