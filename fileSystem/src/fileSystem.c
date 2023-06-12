#include "fileSystem/include/fileSystem.h"

int socketCliente, socketMemoria, fdBitmap, fdBloques;
int cantBloques, tamanioBitmap, tamanioBloques;
t_log * logger, * loggerError; 
t_config * config, * superbloque;
t_bitarray * bitmap;
char * ptrBloques, * ptrBitMap, ** bloques;

int main () {
    logger = iniciarLogger("fileSys.log","File System");
    config = iniciarConfiguracion("filesys.config");
    loggerError = iniciarLogger("errores.log", "File System"); 
    
    atexit(terminarPrograma);

    //conexionMemoria();
    //atexit(cerrarConexion);

    // Se abre el archivo de super-bloque y se agarra la cantidad de bloques y el tamaño de cada bloque.

    superbloque = config_create("superbloque.dat");
    cantBloques = config_get_int_value(superbloque, "BLOCK_COUNT");
    tamanioBitmap = BIT_CHAR(cantBloques);
    tamanioBloques = config_get_int_value(superbloque, "BLOCK_SIZE");

    atexit(cerrarSuperBloque);

    // Se abre el archivo del bitmap de bloques, con las flags para crearla si no existe y escribir y leer en caso de ser necesario.
    // A su vez se crea con permisos para que el usuario pueda leerlas y modificarlas por si las dudas.

    fdBitmap = open("bitmap.dat", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fdBitmap < 0)
        error("No se abrio correctamente el archivo %s; error: %s", "bitmap.dat", strerror(errno));
    atexit(cerrarArchivoBitmap);
    
    if (ftruncate(fdBitmap, tamanioBitmap) < 0)
        error("No se pudo expandir correctamente el archivo %s; error: %s", "bitmap.dat", strerror(errno));

    // mmap es un asco, es lo peor de la galaxia.
    // Se genera un mmap, vinculado directamente al archivo de bitmap, para poder modificarlo directamente.

    // Explicación tecnica: AAAAAAAAAAAAAAAAAAAAAAA
    // Mentira, el mmap, por lo que entiendo, es una función que aloca una cantidad de memoria, según el segundo parametro, y la puede vincular
    // a un archivo, en este caso el del bitmap, para poder modificarlo activamente en memoria.
    // Para hacerlo funcionar creo un bitarray que es una forma de manejar una cantidad de bits especificos, y lo vinculo con el string que 
    // maneja el archivo de bitmap.

    ptrBitMap = mmap(0, tamanioBitmap, PROT_WRITE | PROT_READ, MAP_SHARED, fdBitmap, 0);
    if (ptrBitMap == MAP_FAILED) 
        error("No se mapeo correctamente el bitmap; error: %s", strerror(errno));
    atexit(cerrarMMapBitmap);
    bitmap = bitarray_create_with_mode(ptrBitMap, cantBloques, LSB_FIRST);
    atexit(cerrarBitmap);

    fdBloques = open("bloques.dat", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fdBloques < 0)
        error("No se abrio correctamente el archivo %s; error: %s", "bloques.dat", strerror(errno));
    atexit(cerrarArchivoBloques);

    if (ftruncate(fdBloques, cantBloques * tamanioBloques) < 0)
        error("No se pudo expandir correctamente el archivo %s; error: %s", "bloques.dat", strerror(errno));

    ptrBloques = mmap(0, tamanioBloques * cantBloques, PROT_WRITE | PROT_READ, MAP_SHARED, fdBloques, 0);
    if (ptrBloques == MAP_FAILED) 
        error("No se mapeo correctamente el bitmap; error: %s", strerror(errno));
    atexit(cerrarMMapBloques);
    
    bloques = malloc(cantBloques * sizeof (char *));

    for (int i = 0; i < cantBloques; i++) {
        *(bloques + i) = &ptrBloques[i * tamanioBloques];
    }

    if (mkdir ("directorioFCB", S_IRUSR | S_IWUSR | S_IXUSR) == -1 && errno != EEXIST)
        error ("No se pudo crear o verificar que exista el directorio de FCBs, error: %s", strerror (errno));
    
    /* Pruebas de Crear, abrir, y truncar archivo.
    int retCode;
    retCode = (access("directorioFCB/prueba.fcb", F_OK)) ? crearArchivo ("prueba") : 0;
    if (retCode < 0) error ("Se creo archivo incorrectamente, codigo de error: %d, error: %s", retCode, strerror(errno));
    fcb_t * pruebaFCB = abrirArchivo ("prueba");
    if (pruebaFCB == NULL) error ("No se pudo abrir el archivo %s.", "prueba");

    retCode = copiarABloque (1, "prueba", 6);
    if (retCode < 0) error ("No se pudo copiar correctamente la informacion a los bloques");
    retCode = copiarABloque (2, "prueba", 6);
    if (retCode < 0) error ("No se pudo copiar correctamente la informacion a los bloques");
    eliminarBloque (1);

    retCode = truncarArchivo (pruebaFCB, tamanioBloques * 8);
    if (retCode < 0) error ("No se pudo truncar correctamente el archivo %s, codigo %d.", pruebaFCB->nombre, retCode);
    
    retCode = truncarArchivo (pruebaFCB, 0);
    if (retCode < 0) error ("No se pudo truncar correctamente el archivo %s, codigo %d.", pruebaFCB->nombre, retCode);
    */
    msync(ptrBloques, tamanioBloques * cantBloques, MS_SYNC);
    msync(ptrBitMap, tamanioBitmap, MS_SYNC);

    //escucharAlKernel();
    exit(0);
}

int copiarABloque (uint32_t numBloque, char * texto, uint32_t longDeTexto) {
    if (longDeTexto > (uint32_t) tamanioBloques)
        return -1;
    for (uint32_t i = 0; i < longDeTexto; i++) 
        bloques[numBloque][i] = texto[i];

    return 0;
}

void cerrarConexion () { close (socketMemoria); }
void cerrarSuperBloque () { config_destroy (superbloque); }
void cerrarBitmap () { bitarray_destroy(bitmap); }
void cerrarMMapBitmap () { munmap(ptrBitMap, tamanioBitmap); }
void cerrarArchivoBitmap () { close (fdBitmap); }
void cerrarMMapBloques () { munmap(ptrBloques, tamanioBloques); }
void cerrarArchivoBloques () { close (fdBloques); }