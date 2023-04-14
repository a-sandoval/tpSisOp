#ifndef FILE_SYS_MEMH
#define FILE_SYS_MEM_H

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/config.h>


#include "shared/include/utilsCliente.h" 
#include "shared/include/utilsServidor.h" 
#include "shared/include/manejoListas.h"

extern t_log *logger;

int conectarseAMemoria(t_config*); 



#endif