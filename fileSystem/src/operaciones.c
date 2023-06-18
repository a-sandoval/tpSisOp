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

char * leerArchivo (fcb_t * archivo, uint32_t puntero, uint32_t tamanio) {
    // Bloque Inicial: Si es 1 entonces es el puntero directo, sino es el bloque n - 2 (empezando desde 0).
    uint32_t bloqueInicial = puntero / tamanioBloques + 1;
    // Bloque Final: Idem.
    uint32_t bloqueFinal = (puntero + tamanio) / tamanioBloques + 1;
    // Puntero en bloque: Si el puntero es mayor a 64 bytes, se tiene que sacar el resto de la division para saber
    // donde estaria en el bloque inicial.
    uint32_t punteroEnBloque = puntero % tamanioBloques;
    
    // Tamaño del primer bloque: Si nada mas se accede a un solo bloque entonces el tamaño va a ser el tamaño del
    // primer bloque, sino va a ser 64 bytes menos el puntero en bloque.
    uint32_t tamanioDelPrimerBloque = 
        (bloqueInicial == bloqueFinal)  ? tamanio
                                        : tamanioBloques  - punteroEnBloque;
    // Tamaño del ultimo bloque: Si sumamos el puntero por el tamaño nos va a dar el ultimo valor del puntero.
    // Por lo tanto el resto de esto por 64 bytes nos va a dar el mismo valor pero dentro del ultimo bloque,
    // que es igual al tamaño del ultimo bloque empezando desde 0.
    uint32_t tamanioDelBloqueFinal = (punteroEnBloque + tamanio) % tamanioBloques;
    char *data = malloc (sizeof (char) * tamanio);

    // Direccion del primer bloque: La direccion del primer bloque va a ser, o el puntero directo, o un puntero
    // dentro del puntero indirecto.
    uint32_t direccionBloque = 
        (bloqueInicial == 1) ? archivo->ptrDirecto 
                             : direccionIndirectaAReal (archivo->ptrIndirecto, bloqueInicial - 2);
                            // Se le resta dos para considerar el puntero directo.
    // Caso 1: Me solicitan leer menos de 1 bloque con el puntero inicial entre 0 y 63.
    // Nada mas se lee el primer bloque, y se lee desde el puntero hasta el final del primer bloque o menos.
    // Esto va a pasar siempre.
    for (uint32_t i = punteroEnBloque; i < tamanioDelPrimerBloque + punteroEnBloque; i++) 
        data[i - punteroEnBloque] = bloques[direccionBloque][i];
    // Caso 2: Me solicitan leer mas de un bloque (tamaño + puntero) es mayor a 64 bytes.
    if (bloqueFinal > bloqueInicial) {
        uint32_t desplazamiento = tamanioDelPrimerBloque;
        // Si la cantidad de bloques a leer es mayor a 2, entonces los bloques entre medios van a ser leidos por 
        // completo. Se realiza un for loop anidado para leer cada bloque intermedio.
        for (uint32_t i = bloqueInicial - 1; i < bloqueFinal - 2; i++) {
            for (uint32_t j = 0; j < (uint32_t) tamanioBloques; j++) 
                data[j + desplazamiento] = bloques[direccionIndirectaAReal (archivo->ptrIndirecto, i)][j];
            desplazamiento += tamanioBloques;
        }
        // Si o si va a haber un bloque final y este va a tener un valor menor o igual al tamaño del bloque.
        for (uint32_t i = 0; i < tamanioDelBloqueFinal; i++) 
            data[i + desplazamiento] = bloques[direccionIndirectaAReal (archivo->ptrIndirecto, bloqueFinal)][i];
    }
    return data;
}
// Casos de ejemplo:
/**
 * Caso 1: leerArchivo (archivoX = { "Tantos", 0, 1, 257 }, 12, 120)
 *  bloqueInicial = 1, bloqueFinal = 3, punteroEnBloque = 12, tamañoDelPrimerBloque = 52, tamañoDelUltimoBloque = 4
 * 
*/

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
    return direccionIndirectaAReal (archivo->ptrIndirecto, ptrEnBloque - TAMANIO_PUNTERO);
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