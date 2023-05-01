#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/global.h"
#include<commons/collections/list.h>
#include "shared/include/configuraciones.h"


#include <stdio.h>
#include <stdlib.h>


void cicloDeInstruccion();
void fetch();
void decode();
void execute();

void set(char* , char* );
int obtenerTiempoEspera();



char *listaComandos[] = {

    [SET] = "SET",
    [MOV_IN] = "MOV_IN",
    [MOV_OUT] = "MOV_OUT", 
    [IO] = "I/O",
    [F_OPEN] = "F_OPEN",
    [F_CLOSE] = "F_CLOSE", 
    [F_SEEK] = "F_SEEK",
    [F_READ] = "F_READ",
    [F_WRITE] = "F_WRITE", 
    [F_TRUNCATE] = "F_TRUNCATE",
    [WAIT] = "WAIT",
    [SIGNAL] = "SIGNAL",
    [CREATE_SEGMENT] = "CREATE_SEGMENT",
    [DELETE_SEGMENT] = "DELETE_SEGMENT",
    [YIELD] = "YIELD",
    [EXIT] = "EXIT", 

};





#endif 