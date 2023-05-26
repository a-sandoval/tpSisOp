#ifndef FILE_SYS_H
#define FILE_SYS_H

/**
 * @addtogroup FileSystem
 * @{
 * 
 * @file fileSystem.h
 * @authors To-The-End
 */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>

#include "shared/include/utilsCliente.h" 
#include "shared/include/configuraciones.h"

#include "fileSystem/include/servidorKernel.h"

extern int socketCliente;
extern int socketMemoria;
extern t_log * logger; 
extern t_log * loggerError; 
extern t_config * config; 
extern t_config * superbloque;
extern t_bitarray * bitmap;

#endif


