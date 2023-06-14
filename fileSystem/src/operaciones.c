#include <fileSystem/include/operaciones.h>

int crearArchivo (char * nombre) {
    char * pathArchivo = string_from_format ("%s/%s.fcb", pathFCBs, nombre);
    if (!access (pathArchivo, F_OK)) return -1;
    int tempArchivo = open (pathArchivo, O_CREAT, S_IRUSR | S_IWUSR);
    if (tempArchivo < 0) return -2;
    close (tempArchivo);
    t_config * archivoNuevo = config_create (pathArchivo);
    if (!archivoNuevo) return -3;
    config_set_value (archivoNuevo, "NOMBRE_ARCHIVO", nombre);
    config_set_value (archivoNuevo, "TAMANIO_ARCHIVO", "0");
    char * valorDefault = string_from_format ("%d", UINT32_MAX);
    config_set_value (archivoNuevo, "PUNTERO_DIRECTO", valorDefault);
    config_set_value (archivoNuevo, "PUNTERO_INDIRECTO", valorDefault);
    if (config_save (archivoNuevo) == -1) return -4;
    config_destroy (archivoNuevo);
    free (valorDefault);
    free (pathArchivo);
    return 0;
}

fcb_t * abrirArchivo (char * nombre) {
    char * pathArchivo = string_from_format ("%s/%s.fcb", pathFCBs, nombre);
    if (access (pathArchivo, F_OK)) return NULL;
    
    t_config * archivo = config_create (pathArchivo);
    if (!archivo) return NULL;
    fcb_t * archivoFCB = malloc (sizeof (fcb_t)); 
    archivoFCB->nombre = string_duplicate (config_get_string_value (archivo, "NOMBRE_ARCHIVO"));
    archivoFCB->tamanio = config_get_int_value (archivo, "TAMANIO_ARCHIVO");
    archivoFCB->ptrDirecto = config_get_int_value (archivo, "PUNTERO_DIRECTO");
    archivoFCB->ptrIndirecto = config_get_int_value (archivo, "PUNTERO_INDIRECTO");

    return archivoFCB;
}

int truncarArchivo (fcb_t * archivo, uint32_t tamanio) {
    // Cantidad de bloques asignados al archivo de antemano. 
    int cantBloquesAsignados = CANT_BLOQUES (archivo->tamanio);
    // Cantidad de bloques a asignar.
    int cantBloquesAAsignar = CANT_BLOQUES (tamanio);
    // Si ya se asignaron los bloques necesarios terminar.
    if (cantBloquesAsignados == cantBloquesAAsignar) return 0;
    // Si la cantidad requerida es mayor a la que permite acceder el puntero indirecto y el puntero directo terminar con error.
    if (cantBloquesAAsignar > tamanioBloques / TAMANIO_PUNTERO + 1) return -7;

    // Caso 1: Hay que asignar bloques.
    if (tamanio > archivo->tamanio) {
        // Si no hay bloques asignados, se le asigna un bloque directo y uno de punteros (indirecto).
        if (!archivo->tamanio) {
            archivo->ptrDirecto = proximoBloqueLibre (), cantBloquesAsignados++;
            archivo->ptrIndirecto = proximoBloqueLibre ();
            // Si no hay bloques disponibles para asignar, se termina con error.
            if (archivo->ptrDirecto == UINT32_MAX || archivo->ptrIndirecto == UINT32_MAX) return -5;
            archivo->tamanio = tamanioBloques;
        }

        // Buscar espacio libre en el puntero indirecto.
        uint32_t ptr = espacioParaGuardarPuntero (archivo);
        // Si el puntero indirecto no tiene espacio libre, terminar con error.
        if (ptr >= UINT32_MAX - 1) return -1;

        // Ciclo: Termina cuando se asignaron los bloques requeridos.
        while (cantBloquesAsignados < cantBloquesAAsignar) { 
            uint32_t proxBloque = proximoBloqueLibre ();
            // Si no hay bloque disponible, terminar con error.
            if (proxBloque == UINT32_MAX) return -2; 
            // Si se falla al copiar el puntero del bloque libre al puntero indirecto, se termina con error.
            if (asignarBloqueAArchivo (archivo, proxBloque) < 0) return -3;
            cantBloquesAsignados++, archivo->tamanio += tamanioBloques;
        }
    }

    // Caso 2: Hay que eliminar bloques.
    else {
        // Eliminar bloques si hay en el puntero indirecto y hasta el anteultimo.
        uint32_t ultBloque = ultimoBloqueDeArchivo (archivo);
        while (cantBloquesAsignados > cantBloquesAAsignar + 1 && ultBloque != archivo->ptrDirecto) {
            eliminarBloque (ultBloque);
            log_debug (logger, "Bloque %d eliminado.", ultimoBloqueDeArchivo (archivo));
            cantBloquesAsignados--, archivo->tamanio -= tamanioBloques;
            ultBloque = ultimoBloqueDeArchivo (archivo);
        }

        // Segun cada caso, el ultimo puede ser el puntero directo o uno del puntero indirecto.
        if (cantBloquesAAsignar == 0 && archivo->tamanio > 0) {
            eliminarBloque (archivo->ptrDirecto);
            archivo->ptrDirecto = 0;
            eliminarBloque (archivo->ptrIndirecto);
            archivo->ptrIndirecto = 0;
        }
        else if (ultimoBloqueDeArchivo (archivo) != archivo->ptrDirecto)
            eliminarBloque (ultimoBloqueDeArchivo (archivo));
        else return -6;
    }
    archivo->tamanio = tamanio;
    msync(ptrBloques, tamanioBloques * cantBloques, MS_SYNC);
    msync(ptrBitMap, tamanioBitmap, MS_SYNC);
    if (actualizarFCB (archivo) < 0) return -4;
    return 0;

}

int leerArchivo (fcb_t archivo);
int escribirArchivo (fcb_t archivo);

int asignarBloqueAArchivo (fcb_t * archivo, uint32_t ptr) {
    uint32_t ptrAasignar = espacioParaGuardarPuntero (archivo);
    if (ptrAasignar >= (uint32_t) tamanioBloques - TAMANIO_PUNTERO) return -1;
    uint8_t ptrDeconstruido [4] = {
        ptr >> 24, (ptr >> 16) & 255, (ptr >> 8) & 255, ptr & 255
    };
    log_debug (logger, "Puntero Deconstruido: %d %d %d %d", ptrDeconstruido[0], ptrDeconstruido[1], ptrDeconstruido[2], ptrDeconstruido[3]);
    for (int j = 0; j < 4; j++)
        bloques[archivo->ptrIndirecto][ptrAasignar + j] = (char) ptrDeconstruido[j];
    return 0;
}

uint32_t ultimoBloqueDeArchivo (fcb_t * archivo) {
    uint32_t ptrEnBloque = espacioParaGuardarPuntero (archivo);
    if (ptrEnBloque >= UINT32_MAX - 1) return UINT32_MAX;
    if (ptrEnBloque == 0) return archivo->ptrDirecto;
    uint8_t ptrDeconstruido[TAMANIO_PUNTERO] = {0, 0, 0, 0};
    for (int j = -TAMANIO_PUNTERO; j < 0; j++) {
        ptrDeconstruido[j + TAMANIO_PUNTERO] = (uint8_t) bloques[archivo->ptrIndirecto][ptrEnBloque + j];
    }
    uint32_t ptr = 
        (ptrDeconstruido[0] << 24) + (ptrDeconstruido[1] << 16) + (ptrDeconstruido[2] << 8) + ptrDeconstruido[3];
    return ptr;
}

uint32_t espacioParaGuardarPuntero (fcb_t * archivo) {
    uint32_t bloquesAsignados = CANT_BLOQUES(archivo->tamanio);
    // Se puede asignar el puntero directo.
    if (bloquesAsignados == 0) return UINT32_MAX - 1;
    // No se puede asignar nada.
    if (bloquesAsignados >= (uint32_t) MAX_BLOQUES) return UINT32_MAX;
    // Se puede asignar la ubicacion en la cual estaria el ultimo puntero si no hubiera puntero directo.
    return (bloquesAsignados - 1) * TAMANIO_PUNTERO;
}

int actualizarFCB (fcb_t * archivo) {
    char * pathArchivo = string_from_format ("%s/%s.fcb", pathFCBs, archivo->nombre);
    if (access (pathArchivo, F_OK)) return -1;
    t_config * archivoNuevo = config_create (pathArchivo);
    if (!archivoNuevo) return -3;
    char * tamanio = string_from_format ("%d", archivo->tamanio);
    char * ptrDirecto = string_from_format ("%d", archivo->ptrDirecto);
    char * ptrIndirecto = string_from_format ("%d", archivo->ptrIndirecto);
    config_set_value (archivoNuevo, "TAMANIO_ARCHIVO", tamanio);
    config_set_value (archivoNuevo, "PUNTERO_DIRECTO", ptrDirecto);
    config_set_value (archivoNuevo, "PUNTERO_INDIRECTO", ptrIndirecto);
    if (config_save (archivoNuevo) == -1) return -4;
    config_destroy (archivoNuevo);
    free (tamanio);
    free (ptrDirecto);
    free (ptrIndirecto);
    free (pathArchivo);
    return 0;
}