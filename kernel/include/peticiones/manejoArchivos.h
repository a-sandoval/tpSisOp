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

extern t_log* logger;
extern int conexionAFS;

void solicitarArchivoFS(char* nombreArchivo);
fcb_t* recibirFCB(char* archivo);
void agregarArchivoATG(fcb_t* nuevoArchivo);
extern t_contexto* contextoEjecucion;


#endif