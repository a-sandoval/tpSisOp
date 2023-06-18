#include <fileSystem/include/bloques.h>


uint32_t proximoBloqueLibre () {
    for (uint32_t i = 0; i < (uint32_t) cantBloques; i++) 
        if (!bitarray_test_bit (bitmap, i)) {
            bitarray_set_bit (bitmap, i);
            return i;
        }
    return UINT32_MAX;
}


void eliminarBloque (uint32_t ptr) {
    bitarray_clean_bit (bitmap, ptr);
    for (int i = 0; i < tamanioBloques; i++)
        bloques[ptr][i] = '\0';
}

int copiarABloque (uint32_t numBloque, char * texto, uint32_t longDeTexto) {
    if (longDeTexto > (uint32_t) tamanioBloques)
        return -1;
    for (uint32_t i = 0; i < longDeTexto; i++) 
        bloques[numBloque][i] = texto[i];

    return 0;
}

uint32_t direccionIndirectaAReal (uint32_t ptrIndirecto, uint32_t bloqueABuscar) {
    uint8_t direccionDeconstruida[TAMANIO_PUNTERO];
    for (int i = bloqueABuscar * TAMANIO_PUNTERO; i < TAMANIO_PUNTERO; i++)
        direccionDeconstruida[i - bloqueABuscar] = bloques[ptrIndirecto][i];
    uint32_t direccion = 0;
    for (int i = 1; i <= TAMANIO_PUNTERO; i++)
        direccion += (direccionDeconstruida[i] << ((TAMANIO_PUNTERO - i) * 8));
    return direccion;
}