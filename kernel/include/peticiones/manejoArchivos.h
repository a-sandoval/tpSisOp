#ifndef MANEJO_ARCHIVOS_PLAZO_H
#define MANEJO_ARCHIVOS_PLAZO_H

#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdlib.h> 
#include <stdint.h>
#include "shared/include/utilsCliente.h"
#include "shared/include/global.h"
#include "shared/include/contextoEjecucion.h"
#include "kernel/include/peticiones/pcb.h"

typedef struct {
    char * nombre;
    uint32_t tamanio;
    uint32_t ptrDirecto;
    uint32_t ptrIndirecto;
} fcb_t;

typedef struct{
    
    fcb_t* fcb;
    uint32_t punteroArch;
    
}t_archivoProceso;
typedef struct{

    fcb_t* fcb;
    t_list* colaBloqueados;

}t_archivo;

extern t_log* logger;
extern int conexionAFS;
extern t_contexto* contextoEjecucion;
extern t_list* tablaGlobalArchivos;

t_archivo* solicitarArchivoFS(char* );
void recibirFCB(t_archivo**);
void agregarArchivoATG(t_archivo*);
void iniciarTablaGlobalDeArchivos();
fcb_t* deserializarFCB();
bool estaEnLaTablaGlobal(char* nombreArchivo);
t_archivo* obtenerArchivoDeTG(char* nombreArchivo);
void eliminarArchivo(t_archivo* archivo);
void solicitarTruncadoDeArchivo(fcb_t* fcb, int tamanio);
t_archivoProceso* obtenerArchivoDeProceso(t_pcb* proceso, char* nombreArchivo);
void desbloquearProcesoPorArchivo();
void quitarArchivo(t_pcb* proceso, char* nombreArchivo);
void eliminarArchivoDeTabla(void* archivo);

#endif