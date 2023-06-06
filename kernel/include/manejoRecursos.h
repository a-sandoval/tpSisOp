#ifndef MANEJO_RECURSOS_PLAZO_H
#define MANEJO_RECURSOS_PLAZO_H

#include <commons/collections/list.h>
#include <stdlib.h> 
#include "pcb.h"
#include "planificacion.h"

//Variables de manejo de recursos
extern int *instanciasRecursos;
extern t_list *recursos;
extern char **nombresRecursos;
extern int cantidadRecursos;

void crearColasBloqueo();
int indiceRecurso(char *recurso);
int tamanioArrayCharDoble(char**arreglo);
void closurePCB(void* pcbActual);
void closureMatriz(void* colaBloqueados);
void destruirInstanciasRecursos();
void destruirArrayCharDoble(char**array);
void colaBloqueadosDestroyer(void* colaBloqueados);
void destruirRecursos();
void liberarColasBloqueo();

#endif