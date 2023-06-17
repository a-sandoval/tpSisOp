#ifndef MANEJO_ARCHIVOS_PLAZO_H
#define MANEJO_ARCHIVOS_PLAZO_H

#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdlib.h> 
#include <stdint.h>
#include "shared/include/utilsCliente.h"
#include "shared/include/global.h"
#include "shared/include/contextoEjecucion.h"

typedef struct {
    char * nombre;
    uint32_t tamanio;
    uint32_t ptrDirecto;
    uint32_t ptrIndirecto;
} fcb_t;

typedef struct{

    fcb_t* fcb;
    t_list* colaBloqueados;
    int colaBoqueadosSize;

}t_archivo;

extern t_log* logger;
extern int conexionAFS;
extern t_contexto* contextoEjecucion;

t_archivo* solicitarArchivoFS(char* nombreArchivo);
void recibirFCB(t_archivo** nuevoArchivo);
void agregarArchivoATG(t_archivo* nuevoArchivo);


void deserializarFCB(t_archivo** nuevoArchivo);
void recibirFCB(t_archivo** nuevoArchivo);



#endif