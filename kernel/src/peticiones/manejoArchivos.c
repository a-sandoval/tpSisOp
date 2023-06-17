#include "include/peticiones/manejoArchivos.h"

t_list* tablaGlobalArchivos;

void iniciarTablaGlobalDeArchivos(){
    tablaGlobalArchivos = list_create();
}


t_archivo*  solicitarArchivoFS(char* nombreArchivo){

    t_paquete* peticion = crearPaquete();
    t_archivo* nuevoArchivo = malloc(sizeof(t_archivo));
    nuevoArchivo->fcb->nombre = nombreArchivo;

    log_info(logger, "PID: <PID> - Abrir Archivo: <NOMBRE ARCHIVO>", nombreArchivo);

    peticion->codigo_operacion = F_OPEN;
    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*string_length(nombreArchivo));
    enviarPaquete(peticion, conexionAFS);

    int respuesta = recibirOperacion(conexionAFS);

    switch (respuesta){
        case MENSAJE: // en este caso el archivo no existe y tiene que crearlo
                    peticion->codigo_operacion = F_CREATE; // se sobreentiende que es con tamanio 0
                    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*string_length(nombreArchivo));
                    enviarPaquete(peticion, conexionAFS);
        case PAQUETE: // el archivo ya existe y solo me lo manda
                    recibirFCB(nuevoArchivo);
                    agregarArchivoATG(nuevoArchivo);
        break;
    }

    return nuevoArchivo;
}

void recibirFCB(t_archivo** nuevoArchivo){
    //recibo el fcb

    deserializarFCB(nuevoArchivo);

    (*nuevoArchivo)->colaBloqueados = list_create();
    (*nuevoArchivo)->colaBoqueadosSize = 0;
    (*nuevoArchivo)->colaBoqueadosSize++;

}

void deserializarFCB(t_archivo** nuevoArchivo){
    int size, desplazamiento = 0;
	void * buffer;
    int tamanio;
    fcb_t* fcb;
    fcb->nombre = (*nuevoArchivo)->fcb->nombre;

	buffer = recibirBuffer(socket, &size);

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

    //ademas tengo que poner que archivo la esta utilizando y ver el tema de la cola de bloqueados
    list_add(tablaGlobalArchivos, nuevoArchivo);
}


