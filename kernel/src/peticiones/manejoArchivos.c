#include "kernel/include/peticiones/manejoArchivos.h"

t_list* tablaGlobalArchivos;

void iniciarTablaGlobalDeArchivos(){
    tablaGlobalArchivos = list_create();
}


t_archivo*  solicitarArchivoFS(char* nombreArchivo){

    t_paquete* peticion = crearPaquete();
    t_archivo* nuevoArchivo = malloc(sizeof(t_archivo));
    nuevoArchivo->fcb->nombre = nombreArchivo;

    log_info(logger, "PID: %d - Abrir Archivo: %s",contextoEjecucion->pid,nombreArchivo);

    peticion->codigo_operacion = FOPEN;
    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*string_length(nombreArchivo));
    enviarPaquete(peticion, conexionAFS);

    int respuesta = recibirOperacion(conexionAFS);

    switch (respuesta){
        case MENSAJE: // en este caso el archivo no existe y tiene que crearlo
                    peticion->codigo_operacion = FCREATE; // se sobreentiende que es con tamanio 0
                    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*string_length(nombreArchivo));
                    enviarPaquete(peticion, conexionAFS);
        case PAQUETE: // el archivo ya existe y solo me lo manda
                    recibirFCB(&nuevoArchivo);
                    agregarArchivoATG(nuevoArchivo);
        break;
    }

    return nuevoArchivo;
}



void recibirFCB(t_archivo** nuevoArchivo){
    //recibo el fcb

    deserializarFCB(nuevoArchivo);

    (*nuevoArchivo)->colaBloqueados = list_create();
    (*nuevoArchivo)->colaBloqueadosSize = 0;
    (*nuevoArchivo)->colaBloqueadosSize++;

}

void deserializarFCB(t_archivo** nuevoArchivo){
    int size, desplazamiento = 0;
	void * buffer;
    int tamanio;
    fcb_t* fcb = malloc(sizeof(fcb));
    fcb->nombre = (*nuevoArchivo)->fcb->nombre;

	buffer = recibirBuffer(conexionAFS, &size);

    // Desplazamiento: tamanio del archivo
    desplazamiento += sizeof(int);
    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(&(fcb->tamanio), buffer + desplazamiento, tamanio);
    desplazamiento += sizeof(contextoEjecucion->pid) + sizeof(int);

    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(&(fcb->ptrDirecto), buffer + desplazamiento, tamanio);
    desplazamiento += sizeof(contextoEjecucion->pid) + sizeof(int);

    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(&(fcb->ptrIndirecto), buffer + desplazamiento, tamanio);
    desplazamiento += sizeof(contextoEjecucion->pid) + sizeof(int);

    (*nuevoArchivo)->fcb = fcb;
    free(fcb);


}


void agregarArchivoATG(t_archivo* nuevoArchivo){

    list_add(tablaGlobalArchivos, nuevoArchivo);
}

void eliminarArchivo(t_archivo* archivo){
    //espero que esto este bien
    free(archivo->fcb);
    destruirListaPCB(archivo->colaBloqueados);
    free(archivo);
}

bool estaEnLaTablaGlobal(char* nombreArchivo){

    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivo* archivoAux = malloc(sizeof(t_archivo));


    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(tablaGlobalArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            eliminarArchivo(archivoAux);
            return true;
        }
    }
    eliminarArchivo(archivoAux);
    return false;
}

t_archivo* obtenerArchivo(char* nombreArchivo){

    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivo* archivoAux = malloc(sizeof(t_archivo));


    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(tablaGlobalArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            return archivoAux; //no haria falta otra opcion porque si llegue a esta funcion, el archivo esta
        }
    }
}