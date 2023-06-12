#ifndef FCB_H
#define FCB_H

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/bitarray.h>

#define TAMANIO_PUNTERO 4
#define CANT_BLOQUES(bytes) ((bytes != 0) ? (bytes / tamanioBloques + (bytes < (uint32_t) tamanioBloques)) : 0) 
#define MAX_BLOQUES tamanioBloques / TAMANIO_PUNTERO + 1

extern int cantBloques;
extern int tamanioBitmap;
extern int tamanioBloques;
extern t_log * logger; 
extern t_log * loggerError; 
extern t_config * config; 
extern t_bitarray * bitmap;
extern char * ptrBloques;
extern char * ptrBitMap;
extern char ** bloques;

typedef struct {
    char * nombre;
    uint32_t tamanio;
    uint32_t ptrDirecto;
    uint32_t ptrIndirecto;
} fcb_t;

int crearArchivo (char * nombre);
fcb_t * abrirArchivo (char * nombre);
int truncarArchivo (fcb_t * archivo, uint32_t tamanio);
int leerArchivo (fcb_t archivo);
int escribirArchivo (fcb_t archivo);

int copiarPunteroAPtrIndirecto (fcb_t * archivo, uint32_t ptr);
bool punteroNulo (char ptr[]);
uint32_t proximoBloqueLibre ();
uint32_t conseguirUltimoBloqueDePtrIndirecto (fcb_t * archivo);
uint32_t ptrAsignable (fcb_t * archivo);
void eliminarBloque (uint32_t ptr);
int actualizarFCB (fcb_t * archivo);

#endif 