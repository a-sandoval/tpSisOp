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

    // Se abre el archivo de super-bloque y se agarra la cantidad de bloques y el tamaño de cada bloque.

    superbloque = config_create("superbloque.dat");
    const int cantBloques = config_get_int_value(superbloque, "BLOCK_COUNT");
    const int tamanioBitmap = BIT_CHAR(cantBloques);
    const int tamanioBloques = config_get_int_value(superbloque, "BLOCK_SIZE");

    // Se abre el archivo del bitmap de bloques, con las flags para crearla si no existe y escribir y leer en caso de ser necesario.
    // A su vez se crea con permisos para que el usuario pueda leerlas y modificarlas por si las dudas.

    int fD = open("bitmap.dat", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    struct stat statFD;
    if (fD < 0) {
        log_error(loggerError, "No se abrio correctamente el archivo %s; error: %s", "bitmap.dat", strerror(errno));
        exit(1);
    }
    fstat(fD, &statFD);

    // Si el archivo no existe, procedo a llenarlo con la cantidad que necesita, ¿no deberiamos hacer esto con todos los archivos?

    // Explicación tecnica: El fstat crea un struct llamado stat que contiene información del archivo, por eso se abrio el archivo con 
    // el syscall open(), este stat tiene un parametro que es el tamaño.
    // El bitmap de bloques va a tener un bitmap por cada bloque, por lo que va a tener un octavo de la cantidad de bloques.
    // Por lo tanto, si el tamaño de  este archivo es 0, es porque aun no se creo, y en cuyo caso, como mmap (explicado más adelante) no
    // funciona si el archivo no esta completo, lleno el archivo con el tamaño requerido, con 0s en todo el archivo.
    
    // Nota: Tal vez deberia modularizarlo a una función, y globalizar el archivo.

    if (statFD.st_size != tamanioBitmap) {
        char *filler = malloc (sizeof(char) * tamanioBitmap);
        for (int i = 0; i < tamanioBitmap; i++) 
            filler [i] = 0x00;
        write (fD, filler, tamanioBitmap);
        log_info(logger, "El archivo no existio previamente, se genera con ceros en cada bit.");
        free(filler);
    }

    // mmap es un asco, es lo peor de la galaxia.
    // Se genera un mmap, vinculado directamente al archivo de bitmap, para poder modificarlo directamente.

    // Explicación tecnica: AAAAAAAAAAAAAAAAAAAAAAA
    // Mentira, el mmap, por lo que entiendo, es una función que aloca una cantidad de memoria, según el segundo parametro, y la puede vincular
    // a un archivo, en este caso el del bitmap, para poder modificarlo activamente en memoria.
    // Para hacerlo funcionar creo un bitarray que es una forma de manejar una cantidad de bits especificos, y lo vinculo con el string que 
    // maneja el archivo de bitmap.

    char *ptrBitMap = mmap(0, tamanioBitmap, PROT_WRITE | PROT_READ, MAP_SHARED, fD, 0);
    if (ptrBitMap == MAP_FAILED) {
        log_error(loggerError, "No se mapeo correctamente");
        exit(1);
    }
    bitmap = bitarray_create_with_mode(ptrBitMap, cantBloques, LSB_FIRST);

    // Para comprobar que anda, relleno el archivo con datos basura.
    // Se puede probar con el comando "od -t x1 ./bitmap.dat".
    // Se utiliza el comando msync para sincronizar el archivo con el bitarray.

    for (int i = 0; i < cantBloques; i++)
        ((rand() % 2) - 1) ? bitarray_clean_bit(bitmap, i) : bitarray_set_bit(bitmap, i); 
    msync(ptrBitMap, tamanioBitmap, MS_SYNC);

    FILE *bloques = fopen("bloques.dat","r+w");
    
    /*
    int bloques = open("bloques.dat", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    struct stat statBloques;
    if (bloques < 0) {
        log_error(loggerError, "No se abrio correctamente el archivo %s; error: %s", "bloques.dat", strerror(errno));
        exit(1);
    }
    fstat(bloques, &statBloques);
    

    if (statBloques.st_size != tamanioBloques * cantBloques) {
        char *filler = malloc (sizeof(char) * (tamanioBloques * cantBloques));
        for (int i = 0; i < tamanioBloques * cantBloques; i++) 
            filler [i] = ' ';
        write (bloques, filler, tamanioBloques * cantBloques);
        log_info(logger, "El archivo no existio previamente, se genera vacio.");
        free(filler);
    }
    
    char *(ptrBloques)[cantBloques];
    ptrBloques = mmap(0, tamanioBloques * cantBloques, PROT_WRITE | PROT_READ, MAP_SHARED, bloques, 0);
    if (ptrBloques[i] == MAP_FAILED) {
        log_error(loggerError, "No se mapeo correctamente.");
        exit(1);
    }
    ptrBloques[3] = string_duplicate("Prueba \n");
    msync(ptrBloques, 4096, MS_SYNC);
    */

    escucharAlKernel();

    munmap(ptrBitMap, tamanioBitmap);
    //munmap(ptrBloques, tamanioBloques);
    close(fD);
    close(socketMemoria);
    fclose(bloques);
    config_destroy(superbloque);
    bitarray_destroy(bitmap);
    terminarPrograma();
    exit(0);
}

