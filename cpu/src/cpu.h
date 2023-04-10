#ifndef CPU_H
#define CPU_H

#include <commons/log.h>
#include <commons/config.h>
#include "utilsCliente.h"

t_log* iniciar_logger(void);

void terminar_programa(int conexion, t_log* logger);

#endif /* CPU_H*/
