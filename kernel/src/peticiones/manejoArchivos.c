#include "include/peticiones/manejoArchivos.h"

t_list* tablaGlobalArchivos;

iniciarTablaGlobalDeArchivos(){
    tablaGlobalArchivos = list_create();
}

void solicitarArchivoFS(char* nombreArchivo){

    t_paquete* peticion = crearPaquete();
    fcb_t* nuevoArchivo = malloc(sizeof(fcb_t));

    log_info(logger, "PID: <PID> - Abrir Archivo: <NOMBRE ARCHIVO>", nombreArchivo);

    peticion->codigo_operacion = F_OPEN;
    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*string_length(nombreArchivo));
    enviarPaquete(peticion, conexionAFS);

    int respuesta = recibirOperacion(conexionAFS);

    switch (respuesta){
        case MENSAJE:
                    peticion->codigo_operacion = F_CREATE; // se sobreentiende que es con tamanio 0
                    agregarAPaquete(peticion, nombreArchivo, sizeof(char)*string_length(nombreArchivo));
                    enviarPaquete(peticion, conexionAFS);
        case PAQUETE:
                    nuevoArchivo = recibirFCB(nombreArchivo);
                    agregarArchivoATG(nuevoArchivo);
                    agregarArchivoATablaProceso(nuevoArchivo);
        break;
    }

}

fcb_t* recibirFCB(char* archivo){

}

void agregarArchivoATG(fcb_t* nuevoArchivo){

    //ademas tengo que poner que archivo la esta utilizando y ver el tema de la cola de bloqueados
    list_add(tablaGlobalArchivos, nuevoArchivo);
}

void agregarArchivoATablaProceso(fcb_t* nuevoArchivo){

    list_add_in_index(contextoEjecucion->tablaDeArchivos, 0, (void*)nuevoArchivo);
}
