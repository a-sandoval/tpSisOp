#ifndef FILE_SYS_KERN_SER_H
#define FILE_SYS_KERN_SER_H

#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "fileSystem/include/fcb.h"
#include "fileSystem/include/bloques.h"

void escucharAlKernel();
void ejecutarServidor();
void iterator(void* value); 

extern int socketCliente;

#endif
