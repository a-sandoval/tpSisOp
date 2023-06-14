#ifndef FILE_SYS_KERN_SER_H
#define FILE_SYS_KERN_SER_H

#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "fileSystem/include/operaciones.h"
#include "fileSystem/include/bloques.h"

void escucharAlKernel();
void ejecutarServidor();
void iterator(void* value); 

extern int socketCliente;

typedef enum {
    F_OPEN,
    F_CREATE,
    F_READ,
    F_WRITE,
    F_TRUNCATE
} operacionFS_e;

#endif
