#ifndef MANEJO_ARCHIVOS_PLAZO_H
#define MANEJO_ARCHIVOS_PLAZO_H

#include <commons/collections/list.h>
#include <stdlib.h> 
#include <stdint.h>


typedef struct{

    char* nombre;
    t_list* bloqueados;
    uint32_t tamanio;
    uint32_t puntero; //??????
    //deberia representar propiamente el archivo

}t_fcb;


#endif