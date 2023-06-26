#include "kernel/include/peticiones/manejoArchivos.h"

t_list* tablaGlobalArchivos;

void iniciarTablaGlobalDeArchivos(){
    tablaGlobalArchivos = list_create();
}
/*
void desbloquearProcesoPorArchivo(){
    t_archivo* archivo = malloc(sizeof(t_archivo));
    t_pcb* procesoADesbloquear = malloc(sizeof(t_pcb));

    recibirOperacion(conexionAFS);
    archivo->fcb = deserializarFCB();

    archivo = obtenerArchivoDeTG(fcb->nombre);

    //supuestamente el que quedo primero es el que se bloqueo por esta peticion y los demas llegaron dsps
    procesoADesbloquear = list_get(archivo->colaBloqueados, 0);
    
    procesoADesbloquear->estado = READY;

    log_info(logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", procesoADesbloquear->pid, BLOCK, READY);
    ingresarAReady(procesoADesbloquear);

}*/

t_archivo* solicitarArchivoFS(char* nombreArchivo){

    t_paquete* peticion = crearPaquete();
    t_archivo* nuevoArchivo = malloc(sizeof(t_archivo));
    nuevoArchivo->fcb = malloc (sizeof (fcb_t));
    nuevoArchivo->fcb->nombre = nombreArchivo;

    log_info(logger, "PID: %d - Abrir Archivo: %s",contextoEjecucion->pid,nombreArchivo);

    peticion->codigo_operacion = FOPEN;
    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*strlen(nombreArchivo) + 1);
    enviarPaquete(peticion, conexionAFS);

    int respuesta = recibirOperacion(conexionAFS);

    switch (respuesta){
        case MENSAJE: // en este caso el archivo no existe y tiene que crearlo
                    peticion->codigo_operacion = FCREATE; // se sobreentiende que es con tamanio 0
                    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*strlen(nombreArchivo) + 1);
                    enviarPaquete(peticion, conexionAFS);
                    nuevoArchivo->fcb = deserializarFCB();
                    nuevoArchivo->colaBloqueados = list_create();
                    agregarArchivoATG(nuevoArchivo);
                    break;
        case PAQUETE: // el archivo ya existe y solo me lo manda
                    nuevoArchivo->fcb = deserializarFCB();
                    nuevoArchivo->colaBloqueados = list_create();
                    agregarArchivoATG(nuevoArchivo);
                    break;
    }

    return nuevoArchivo;
}


fcb_t* deserializarFCB(){
    int size, desplazamiento = 0;
	void * buffer;
    int tamanio;
    fcb_t* fcb = malloc(sizeof(fcb));

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

    return fcb;

}


void agregarArchivoATG(t_archivo* nuevoArchivo){

    list_add(tablaGlobalArchivos, nuevoArchivo);
}



bool estaEnLaTablaGlobal(char* nombreArchivo){

    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivo* archivoAux = malloc(sizeof(t_archivo));

    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(tablaGlobalArchivos, i);

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
    t_archivoProceso* archivoAux = malloc(sizeof(t_archivo));


    for(int i=0; i<cantArchivos; i++){
        archivoAux = list_get(proceso->tablaDeArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            return archivoAux; 
        }
        //aunque el archivo deberia estar si o si porque ya lo comprobe antes
    }

    return NULL;
}

void quitarArchivo(t_pcb* proceso, char* nombreArchivo){
    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivoProceso* archivoAux = malloc(sizeof(t_archivoProceso));

    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(proceso->tablaDeArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            list_remove_and_destroy_element(proceso->tablaDeArchivos,i,eliminarArchivoProceso);
        }
    }

}

void solicitarTruncadoDeArchivo(fcb_t* fcb, int tamanio){
    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = FTRUNCATE;
    uint32_t tamanio_u = (uint32_t) tamanio;

    agregarAPaquete(peticion, (void*)fcb->nombre, sizeof(char) * string_length(fcb->nombre));
    agregarAPaquete(peticion, &(fcb->tamanio), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrDirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(fcb->ptrIndirecto), sizeof(uint32_t));
    agregarAPaquete(peticion, &(tamanio_u), sizeof(uint32_t));

    enviarPaquete(peticion, conexionAFS);

}

void solicitarLecturaDeArchivo(t_archivoProceso* archivo, uint32_t dirFisica, uint32_t bytes){
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

    enviarPaquete(peticion, conexionAFS);

}

void solicitarEscrituraDeArchivo(t_archivoProceso* archivo, uint32_t dirFisica, uint32_t bytes){
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

    enviarPaquete(peticion, conexionAFS);

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
