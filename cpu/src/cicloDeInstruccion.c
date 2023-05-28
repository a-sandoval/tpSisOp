#include "cpu/include/cicloDeInstruccion.h"

char *listaComandos[] = {
    [SET] = "SET",
    [MOV_IN] = "MOV_IN",
    [MOV_OUT] = "MOV_OUT", 
    [IO] = "I/O",
    [F_OPEN] = "F_OPEN",
    [F_CLOSE] = "F_CLOSE", 
    [F_SEEK] = "F_SEEK",
    [F_READ] = "F_READ",
    [F_WRITE] = "F_WRITE", 
    [F_TRUNCATE] = "F_TRUNCATE",
    [WAIT] = "WAIT",
    [SIGNAL] = "SIGNAL",
    [CREATE_SEGMENT] = "CREATE_SEGMENT",
    [DELETE_SEGMENT] = "DELETE_SEGMENT",
    [YIELD] = "YIELD",
    [EXIT] = "EXIT"
};
char* instruccionAEjecutar; 
char** elementosInstruccion; 
int instruccionActual; 
int cantParametros;
//nuevos
char* direccionLogica;
int tiempo;//miedo
char* nombreArchivo;
uint32_t posicion;
int cantBytes;
int tamanio;
char* recurso;
uint32_t idSegmento;
t_temporal* rafagaCPU; 
int64_t rafagaCPUEjecutada; 


t_contexto* contextoEjecucion;

void liberarMemoria() {
    for (int i = 0; i <= cantParametros; i++) free(elementosInstruccion[i]);
    free(elementosInstruccion);
}

void cicloDeInstruccion(){

    fetch();//busca la próxima instruccion a ejecutar. Lista en pcb

    decode();//interpreta que instruccion va a ejecutar y si requiere traduccion logica o fisica

    execute();//ejecuta la instruccion 
    
    liberarMemoria();
}

// ------- Funciones del ciclo ------- //
void fetch() { 
    int numInstruccionABuscar = contextoEjecucion->programCounter;
    instruccionAEjecutar = list_get(contextoEjecucion->instrucciones,numInstruccionABuscar); 
    contextoEjecucion->programCounter += 1;
}

void decode(){
    elementosInstruccion = string_n_split(instruccionAEjecutar, 4, " ");
    cantParametros = string_array_size(elementosInstruccion) - 1;
    //instruccionAEjecutar = string_(elementosInstruccion[cantParametros], "\0", "");
    instruccionActual = buscar(elementosInstruccion[0], listaComandos);
}
 
void execute() {

   rafagaCPU= temporal_create(); 
  

    switch(cantParametros) {
        case 0:
            log_info(logger, "PID: %d - Ejecutando: %s ", contextoEjecucion->pid, listaComandos[instruccionActual]);
            break;
        case 1:
            log_info(logger, "PID: %d - Ejecutando: %s -  %s", contextoEjecucion->pid, listaComandos[instruccionActual], elementosInstruccion[1]);
            break;
        case 2:   
            log_info(logger, "PID: %d - Ejecutando: %s -  %s, %s", contextoEjecucion->pid, listaComandos[instruccionActual], elementosInstruccion[1], elementosInstruccion[2]);
            break; 
        case 3:
            log_info(logger, "PID: %d - Ejecutando: %s -  %s, %s, %s", contextoEjecucion->pid, listaComandos[instruccionActual], elementosInstruccion[1], elementosInstruccion[2], elementosInstruccion[3]);
            break; 
    }
    switch(instruccionActual){
        case SET:
            set_c(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case MOV_IN://falta
            mov_in(elementosInstruccion[1], direccionLogica);
            break;
        case MOV_OUT://falta
            mov_out(direccionLogica, elementosInstruccion[1]);
            break;
        case IO://IMPLEMENTAR
            io(tiempo);
            break;
        case F_OPEN://falta
            f_open(nombreArchivo);
            break;
        case F_CLOSE://falta
            f_close(nombreArchivo);
            break;
        case F_SEEK://falta
            f_seek(nombreArchivo, posicion);
            break;
        case F_READ://falta
            f_read(nombreArchivo, direccionLogica, cantBytes);
            break;
        case F_WRITE://falta
            f_write(nombreArchivo, direccionLogica, cantBytes);
            break;
        case F_TRUNCATE://falta
            f_truncate(nombreArchivo, tamanio);
            break;
        case WAIT://IMPLEMENTAR
            wait_c(recurso);
            //un recurso puede ser un archivo, memoria reservada, semáforos, sockets, etc
            break;
        case SIGNAL://IMPLEMENTAR
            signal_c(recurso);
            break;
        case CREATE_SEGMENT://falta
            create_segment(idSegmento, tamanio);
            break;
        case DELETE_SEGMENT://falta
            delete_segment(idSegmento);
            break;
        case YIELD: //IMPLEMENTADA
            yield_c();
            break;
        case EXIT: //IMPLEMENTADA
            exit_c();
            break;
        default:
            //sleep(1);
            break;
    }
}


void set_c(char* registro, char* valor){
    int tiempoEspera = obtenerTiempoEspera();
    sleep(tiempoEspera); 
    dictionary_remove_and_destroy(contextoEjecucion->registrosCPU, registro, free); 
    dictionary_put(contextoEjecucion->registrosCPU, registro, string_duplicate(valor));
}

int obtenerTiempoEspera(){
    return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

void io(int tiempo){
    temporal_stop(rafagaCPU); 
    
    rafagaCPUEjecutada = temporal_gettime(rafagaCPU);  

    contextoEjecucion->motivoDesalojo->comando = IO;
    contextoEjecucion->motivoDesalojo->parametros[0]= tiempo;
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();


}

void wait_c(char* recurso){
    
    //Si hay un bloqueo, hay que detener la rafaga de CPU
    //temporal_stop(rafagaCPU); 
    contextoEjecucion->motivoDesalojo->comando = WAIT;
    contextoEjecucion->motivoDesalojo->parametros[0] = recurso;
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();
}

void signal_c(char* recurso){

    //Si hay un bloqueo, hay que detener la rafaga de CPU

    //temporal_stop(rafagaCPU); 
    contextoEjecucion->motivoDesalojo->comando = SIGNAL;
    contextoEjecucion->motivoDesalojo->parametros[0] = recurso;
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();
}

void yield_c(){ 
    temporal_stop(rafagaCPU);
    int64_t rafagaCPUEjecutada = temporal_gettime(rafagaCPU);  

    contextoEjecucion->motivoDesalojo->comando = YIELD;
    contextoEjecucion->motivoDesalojo->parametros[0]= NULL;
    contextoEjecucion->motivoDesalojo->parametrosLength = 0;

    enviarContextoActualizado();

    temporal_destroy(rafagaCPU); 
}

void exit_c(){

    temporal_stop(rafagaCPU); 
    temporal_destroy(rafagaCPU); 

    contextoEjecucion->motivoDesalojo->comando = EXIT;
    contextoEjecucion->motivoDesalojo->parametros[0]= NULL;
    contextoEjecucion->motivoDesalojo->parametrosLength = 0;
    enviarContextoActualizado();
}

void f_open(char* nombre){

    contextoEjecucion->motivoDesalojo->comando = F_OPEN;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre;
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();
};

void f_close(char* nombre){

    contextoEjecucion->motivoDesalojo->comando = F_CLOSE;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre;
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();
};

void f_seek(char* nombre, uint32_t puntero){

    contextoEjecucion->motivoDesalojo->comando = F_SEEK;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre + " " + puntero;
    contextoEjecucion->motivoDesalojo->parametrosLength = 2;

    enviarContextoActualizado();
};

void f_read(char* nombre, char* direccionLogica, int cantBytes){

    contextoEjecucion->motivoDesalojo->comando = F_READ;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre + " " + direccionLogica + " " + cantBytes;
    contextoEjecucion->motivoDesalojo->parametrosLength = 3;

    enviarContextoActualizado();
};

void f_write(char* nombre, char* direccionLogica, int cantBytes){

    contextoEjecucion->motivoDesalojo->comando = F_WRITE;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre + " " + direccionLogica + " " + cantBytes;
    contextoEjecucion->motivoDesalojo->parametrosLength = 3;

    enviarContextoActualizado();
};

void f_truncate(char* nombre, int tamanio){

    contextoEjecucion->motivoDesalojo->comando = F_TRUNCATE;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre + " " + tamanio;
    contextoEjecucion->motivoDesalojo->parametrosLength = 2;

    enviarContextoActualizado();
};

void create_segment(uint32_t idSegmento, int tamanio){

    contextoEjecucion->motivoDesalojo->comando = CREATE_SEGMENT;
    contextoEjecucion->motivoDesalojo->parametros[0] = idSegment + " " + tamanio;
    contextoEjecucion->motivoDesalojo->parametrosLength = 2;

    enviarContextoActualizado();
};

void delete_segment(uint32_t idSegmento){

    contextoEjecucion->motivoDesalojo->comando = DELETE_SEGMENT;
    contextoEjecucion->motivoDesalojo->parametros[0] = idSegment 
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();
};


// MANEJO DE CONTEXTO
void enviarContextoActualizado(){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
   
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));

    //agregarInstruccionesAPaquete(paquete, contextoEjecucion->instrucciones);

    agregarRegistrosAPaquete(paquete, contextoEjecucion->registrosCPU);

    //no sabemos listas de que son estas tablas entonces aun no podemos serializar o hay que serializarlo como listas y ver si dsps cambia
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeArchivosSize, sizeof(contextoEjecucion->tablaDeArchivosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeArchivos, contextoEjecucion->tablaDeArchivosSize);
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeSegmentosSize, sizeof(contextoEjecucion->tablaDeSegmentosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeSegmentos, contextoEjecucion->tablaDeSegmentosSize);
    
    agregarMotivoAPaquete(paquete,contextoEjecucion->motivoDesalojo);
    agregarAPaquete(paquete, (void *)&contextoEjecucion->rafagaCPUEjecutada, sizeof(contextoEjecucion->rafagaCPUEjecutada));

    enviarPaquete(paquete, socketCliente);

	eliminarPaquete(paquete);
}

void agregarMotivoAPaquete(t_paquete* paquete, t_motivoDeDesalojo* motivoDesalojo){

    agregarAPaquete(paquete,(void *)&motivoDesalojo->comando, sizeof(motivoDesalojo->comando));

    agregarAPaquete(paquete,(void *)&motivoDesalojo->parametrosLength, sizeof(motivoDesalojo->parametrosLength));
    agregarAPaquete(paquete,(void *)motivoDesalojo->parametros, strlen(motivoDesalojo->parametros) + 1);
}

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones){

    contextoEjecucion->instruccionesLength = list_size(instrucciones);
    
    agregarAPaquete(paquete, &contextoEjecucion->instruccionesLength, sizeof(uint32_t)); //primero envio la cantidad de elementos
    uint32_t i;
    for(i=0;i<contextoEjecucion->instruccionesLength;i++){
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * (strlen(list_get(instrucciones, i)) + 1 ));
    }
}

void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU){
     
    char *AX = dictionary_get(registrosCPU,"AX"); 
    char* BX = dictionary_get(registrosCPU,"BX");
    char* CX = dictionary_get(registrosCPU,"CX");
    char* DX = dictionary_get(registrosCPU,"DX");
    char* EAX = dictionary_get(registrosCPU,"EAX");
    char* EBX = dictionary_get(registrosCPU,"EBX");
    char* ECX = dictionary_get(registrosCPU,"ECX");
    char* EDX = dictionary_get(registrosCPU,"EDX");
    char* RAX = dictionary_get(registrosCPU,"RAX");
    char* RBX = dictionary_get(registrosCPU,"RBX");
    char* RCX = dictionary_get(registrosCPU,"RCX");
    char* RDX = dictionary_get(registrosCPU,"RDX");

    agregarAPaquete(paquete, AX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, BX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, CX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, DX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, EAX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, EBX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, ECX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, EDX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, RAX, sizeof(char) * (16 + 1));
    agregarAPaquete(paquete, RBX, sizeof(char) * (16 + 1));
    agregarAPaquete(paquete, RCX, sizeof(char) * (16 + 1));
    agregarAPaquete(paquete, RDX, sizeof(char) * (16 + 1));

}

int buscar(char *elemento, char **lista) {
    int i = 0;
    for (; strcmp(lista[i], elemento) && i <= string_array_size(lista); i++);
    return (i > string_array_size(lista)) ? -1 : i;
}


void mov_in(char* registro, char* direccionLogica){
};

void mov_out(char* direccionLogica, char* registro){
};  

