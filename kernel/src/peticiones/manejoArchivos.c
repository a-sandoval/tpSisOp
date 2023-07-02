#include "kernel/include/peticiones/manejoArchivos.h"

t_list* tablaGlobalArchivos;
extern pthread_mutex_t mutexFS;
extern estadoProceso estadoAnterior; 

void iniciarTablaGlobalDeArchivos(){
    tablaGlobalArchivos = list_create();
}


fcb_t* deserializarFCB(){
    int size, desplazamiento = 0;
	void * buffer;
    int tamanio;
    fcb_t * fcb = malloc(sizeof(fcb_t));

	buffer = recibirBuffer(conexionAFS, &size);

    memcpy (&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    fcb->nombre = malloc (sizeof (char) * tamanio);
    memcpy(fcb->nombre, buffer + desplazamiento, sizeof(char)*tamanio);
    desplazamiento += tamanio;

    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(&(fcb->tamanio), buffer + desplazamiento, tamanio);
    desplazamiento += sizeof(uint32_t);

    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(&(fcb->ptrDirecto), buffer + desplazamiento, tamanio);
    desplazamiento += sizeof(uint32_t);

    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(&(fcb->ptrIndirecto), buffer + desplazamiento, tamanio);
    desplazamiento += sizeof(uint32_t);
    
    free (buffer);
    return fcb;

}


void agregarArchivoATG(t_archivo* nuevoArchivo){

    list_add(tablaGlobalArchivos, nuevoArchivo);
}


bool estaEnLaTablaGlobal(char* nombreArchivo){

    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivo* archivoAux = malloc(sizeof(t_archivo));

    for(int i=0; i<cantArchivos; i++){
        archivoAux = list_get(tablaGlobalArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            return true;
        }
    }
    return false;
}

bool estaAsignadoAlProceso(char* nombreArchivo, t_pcb* proceso){

    int cantArchivos = list_size(proceso->tablaDeArchivos);
    t_archivoProceso* archivoAux = malloc(sizeof(t_archivoProceso));

    for(int i=0; i<cantArchivos; i++){
        archivoAux = list_get(proceso->tablaDeArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            return true;
        }
    }
    return false;
}

t_archivo* obtenerArchivoDeTG(char* nombreArchivo){

    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivo* archivoAux = malloc(sizeof(t_archivo));

    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(tablaGlobalArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            return archivoAux; 
        }
        //aunque el archivo deberia estar si o si porque ya lo comprobe antes
    }

    return NULL;
}

t_archivoProceso* obtenerArchivoDeProceso(t_pcb* proceso, char* nombreArchivo){

    int cantArchivos = list_size(proceso->tablaDeArchivos);
    t_archivoProceso* archivoAux = crearArchivoProceso();

    //log_debug (logger, "Se busca en proceso %d el archivo abierto \"%s\", con una cantidad de archivos en lista %d", proceso->pid, nombreArchivo, cantArchivos);

    for(int i=0; i<cantArchivos; i++){
        archivoAux = list_get(proceso->tablaDeArchivos, i);
        //log_debug (logger, "Archivo encontrado: %s", archivoAux->fcb->nombre);
        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            return archivoAux; 
        }
    }

    return NULL;
}

t_archivoProceso * crearArchivoProceso () {
    t_archivoProceso * nuevo = malloc (sizeof (t_archivoProceso));
    nuevo->fcb = crearFCB ();
    return nuevo;
}

void quitarArchivo(t_pcb* proceso, char* nombreArchivo){
    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivoProceso* archivoAux = malloc(sizeof(t_archivoProceso));

    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(proceso->tablaDeArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            list_remove(proceso->tablaDeArchivos,i);
            archivoAux->fcb = NULL;
            free (archivoAux);
            return;
        }
    }

}

//   Manejo de solicitudes al FS

t_archivo* solicitarArchivoFS(char* nombreArchivo){

    t_paquete* peticion = crearPaquete();
    t_archivo* nuevoArchivo = malloc(sizeof(t_archivo));
    nuevoArchivo->fcb = malloc (sizeof (fcb_t));
    nuevoArchivo->fcb->nombre = nombreArchivo;

    log_info(logger, "PID: %d - Abrir Archivo: %s",contextoEjecucion->pid,nombreArchivo);

    peticion->codigo_operacion = FOPEN;
    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*strlen(nombreArchivo) + 1);

    pthread_mutex_lock(&mutexFS);

    enviarPaquete(peticion, conexionAFS);
    
    eliminarPaquete (peticion);

    int respuesta = recibirOperacion(conexionAFS);

    switch (respuesta){
        case MENSAJE: // en este caso el archivo no existe y tiene que crearlo
                    peticion = crearPaquete ();
                    peticion->codigo_operacion = FCREATE; // se sobreentiende que es con tamanio 0
                    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*strlen(nombreArchivo) + 1);
                    enviarPaquete(peticion, conexionAFS);
                    eliminarPaquete (peticion);
                    recibirOperacion (conexionAFS);
                    nuevoArchivo->fcb = deserializarFCB();
                    nuevoArchivo->colaBloqueados = list_create();
                    agregarArchivoATG(nuevoArchivo);
                    break;
        case PAQUETE: // el archivo ya existe y solo me lo manda
                    nuevoArchivo->fcb = deserializarFCB();
                    nuevoArchivo->colaBloqueados = list_create();
                    //log_debug (logger, "Recibido FCB: %s %d %d %d", nuevoArchivo->fcb->nombre, nuevoArchivo->fcb->tamanio, nuevoArchivo->fcb->ptrDirecto, nuevoArchivo->fcb->ptrIndirecto);
                    agregarArchivoATG(nuevoArchivo);
                    break;
    }
    pthread_mutex_unlock(&mutexFS);

    return nuevoArchivo;
}

t_paquete* crearPeticionDeTruncadoDeArchivo(fcb_t* fcb, int tamanio){
    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = FTRUNCATE;
    uint32_t tamanio_u = (uint32_t) tamanio;

    agregarAPaquete(peticion, (void*)fcb->nombre, sizeof(char) * string_length(fcb->nombre));
    agregarAPaquete(peticion, &(fcb->tamanio), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrDirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrIndirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(tamanio_u), sizeof(uint32_t));

    return peticion;
}

t_paquete* crearPeticionDeLecturaDeArchivo(t_archivoProceso* archivo, uint32_t dirFisica, uint32_t bytes){
    fcb_t* fcb = archivo->fcb;
    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = FREAD;

    agregarAPaquete(peticion, (void*)fcb->nombre, sizeof(char) * string_length(fcb->nombre));
    agregarAPaquete(peticion, &(fcb->tamanio), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrDirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrIndirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(archivo->punteroArch), sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&(dirFisica), sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&(bytes), sizeof(uint32_t));

    return peticion;

}

t_paquete* crearPeticionDeEscrituraDeArchivo(t_archivoProceso* archivo, uint32_t dirFisica, uint32_t bytes){
    fcb_t* fcb = archivo->fcb;
    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = FWRITE;

    agregarAPaquete(peticion, (void*)fcb->nombre, sizeof(char) * string_length(fcb->nombre));
    agregarAPaquete(peticion, &(fcb->tamanio), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrDirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrIndirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(archivo->punteroArch), sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&(dirFisica), sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&(bytes), sizeof(uint32_t));

    return peticion;
}

fcb_t * crearFCB () {
    fcb_t * nuevo = malloc (sizeof (fcb_t));
    nuevo->nombre = NULL;
    nuevo->tamanio = 0;
    nuevo->ptrDirecto = 0;
    nuevo->ptrIndirecto = 0;
    return nuevo;
}

void eliminarArchivoProceso(void* archivo){
    t_archivoProceso * bleh = (t_archivoProceso *) archivo;
    //espero que esto este bien
    free(bleh->fcb->nombre);
    free(bleh->fcb);
    free(archivo);
}

void eliminarArchivo(t_archivo* archivo){
    //espero que esto este bien
    free(archivo->fcb->nombre);
    free(archivo->fcb);
    destruirListaPCB(archivo->colaBloqueados);
    free(archivo);
}

void peticionConBloqueoAFS(t_paquete* peticion, t_pcb* proceso){

    pthread_mutex_lock(&mutexFS);
    enviarPaquete(peticion, conexionAFS);
    eliminarPaquete (peticion);
    pthread_t respuestaFS_h;

    if (!pthread_create(&respuestaFS_h, NULL, (void *)respuestaPeticionFS, NULL)){
        pthread_join(respuestaFS_h,NULL);
        estimacionNuevaRafaga(proceso); 
        estadoAnterior = proceso->estado;
        proceso->estado = READY;
        loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
        ingresarAReady(proceso);
    } else {
        error("Error en la creacion de hilo para esperar respuesta del FS, Abort");
    }

    pthread_mutex_unlock(&mutexFS);

}

void respuestaPeticionFS(){
    
    int respuesta = recibirOperacion(conexionAFS);

    switch (respuesta){
        case MENSAJE:
            log_debug(logger,"FS termino padre ya te podes desbloquear");
            break;
        default:
            log_debug(logger,"recibi algo que no era un mensaje volve a probar");
            break;
    }

}