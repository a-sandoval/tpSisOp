#include "kernel/include/pcb.h"

void imprimirRegistros(t_dictionary *registros)
{
    log_info(logger, "AX: %s", (char *)dictionary_get(registros, "AX"));
}

