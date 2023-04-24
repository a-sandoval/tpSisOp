#ifndef CPU_H
#define CPU_H

#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/configuraciones.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "shared/include/global.h"
#include<commons/collections/list.h>

int conexionMemoria();
void iterator(char *value);

typedef enum t_comando {

    SET,
    MOV_IN,
    MOV_OUT, 
    IO,
    F_OPEN,
    F_CLOSE, 
    F_SEEK,
    F_READ,
    F_WRITE, 
    F_TRUNCATE,
    WAIT,
    SIGNAL,
    CREATE_SEGMENT,
    DELETE_SEGMENT,
    YIELD,
    EXIT, 

} t_comando; 
typedef struct t_instruccion {

    t_comando comando; 
    char* parametros[3]; 

}t_instruccion; 

#endif 