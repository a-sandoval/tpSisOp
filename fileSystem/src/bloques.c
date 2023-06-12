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
