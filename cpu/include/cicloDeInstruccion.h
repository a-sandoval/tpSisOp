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



#endif 