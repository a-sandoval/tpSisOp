#include "fileSystem/include/fileSystem.h"

int socketCliente;
int socketMemoria;
t_log * logger; 
t_log * loggerError; 
t_config * config; 
t_config * superbloque;
t_bitarray * bitmap;

int main () {
    logger = iniciarLogger("fileSys.log","file-system");
    config = iniciarConfiguracion("filesys.config");
    loggerError = iniciarLogger("errores.log", "File-System"); 
    socketMemoria = conexion("MEMORIA"); 

    superbloque = config_create("superbloque.dat");
    int cantBloques = config_get_int_value(superbloque, "BLOCK_COUNT");

    int fD = open("bitmap.dat", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    struct stat statFD;
    if (fD < 0) {
        log_error(loggerError, "No se abrio correctamente el archivo %s; error: %s", "bitmap.dat", strerror(errno));
        exit(1);
    }
    fstat(fD, &statFD);

    if (statFD.st_size == 0) {
        char *filler = malloc (sizeof(char) * cantBloques/8);
        for (int i = 0; i < cantBloques/8; i++) 
            filler [i] = 0x00;
        write (fD, filler, cantBloques/8);
    }

    char *ptrBitMap = mmap(0, cantBloques/8, PROT_WRITE | PROT_READ, MAP_SHARED, fD, 0);
    if (ptrBitMap == MAP_FAILED) {
        log_error(loggerError, "No se mapeo correctamente");
        exit(1);
    }
    bitmap = bitarray_create_with_mode(ptrBitMap, cantBloques, LSB_FIRST);
    for (int i = 0; i < cantBloques; i++)
        ((rand() % 2) - 1) ? bitarray_clean_bit(bitmap, i) : bitarray_set_bit(bitmap, i); 
    msync(ptrBitMap, cantBloques/8, MS_SYNC);
    for (int i = 0; i < 256; i++) {
        log_info(logger, "%d", bitarray_test_bit(bitmap, i));
    }
    munmap(ptrBitMap, cantBloques/8);
    close(fD);

    //escucharAlKernel();

    close(socketMemoria);
    config_destroy(superbloque);
    bitarray_destroy(bitmap);
    terminarPrograma();
    exit(0);
}






