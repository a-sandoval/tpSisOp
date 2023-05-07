#ifndef CPU_KERN_SER_H
#define CPU_KERN_SER_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>

#include "shared/include/utilsServidor.h"
#include "shared/include/utilsCliente.h"
#include "cpu/include/cicloDeInstruccion.h"

extern t_log* loggerError; 

void escucharAlKernel(); 
void iterator(void *value);

#endif