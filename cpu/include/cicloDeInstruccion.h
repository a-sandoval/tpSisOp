#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include "shared/include/global.h"
#include<commons/collections/list.h>
#include"cpu/include/registros.h"
#include "shared/include/configuraciones.h"
#include "kernel/include/planificacion.h"
#include "cpu/include/registros.h"

void cicloDeInstruccion();
void fetch();
void decode();
void executeCPU();

void set(t_reg , int );
int obtenerTiempoEspera();

#endif 