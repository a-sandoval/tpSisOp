#ifndef FILE_SYS_H
#define FILE_SYS_H

/**
 * @addtogroup FileSystem
 * @{
 * 
 * @file fileSystem.h
 * @authors To-The-End
 */

#include<commons/log.h>
#include<commons/config.h>

#include "shared/include/utilsCliente.h" 
#include "shared/include/configuraciones.h"

#include "fileSystem/include/servidorKernel.h"

extern int socketCliente;
extern int socketMemoria;
extern t_log* logger; 
extern t_log* loggerError; 
extern t_config* config; 

void iterator(void* value);

#endif


