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
#include "shared/include/global.h"

#include "fileSystem/include/servidorKernel.h"

int socketCliente;
int socketMemoria;

void iterator(void* value);

#endif


