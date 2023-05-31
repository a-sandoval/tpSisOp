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

   rafagaCPU = temporal_create(); 


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
            set_c(elementosInstruccion[0], elementosInstruccion[1]);
            break;
        /*
        case MOV_IN:
            mov_in(elementosInstruccion[0], elementosInstruccion[1]);
            break;
        case MOV_OUT:
            mov_out(elementosInstruccion[0], elementosInstruccion[1]);
            break;
        */
        case IO:
            io(elementosInstruccion[0]);
            break;
        case F_OPEN:
            f_open(elementosInstruccion[0]);
            break;
        case F_CLOSE:
            f_close(elementosInstruccion[0]);
            break;
        case F_SEEK:
            f_seek(elementosInstruccion[0], elementosInstruccion[1]);
            break;
        case F_READ:
            f_read(elementosInstruccion[0], elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case F_WRITE:
            f_write(elementosInstruccion[0], elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case F_TRUNCATE:
            f_truncate(elementosInstruccion[0], elementosInstruccion[1]);
            break;
        case WAIT:
            wait_c(elementosInstruccion[0]);
            //un recurso puede ser un archivo, memoria reservada, semáforos, sockets, etc
            break;
        case SIGNAL:
            signal_c(elementosInstruccion[0]);
            break;
        case CREATE_SEGMENT:
            create_segment(elementosInstruccion[0], elementosInstruccion[1]);
            break;
        case DELETE_SEGMENT:
            delete_segment(elementosInstruccion[0]);
            break;
        case YIELD: 
            yield_c();
            break;
        case EXIT:
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

void io(char* tiempo){
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
    contextoEjecucion->rafagaCPUEjecutada = temporal_gettime(rafagaCPU);  

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
    contextoEjecucion->motivoDesalojo->parametros[0]= "SUCCESS";
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;
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

void f_seek(char* nombre, char* puntero){

    contextoEjecucion->motivoDesalojo->comando = F_SEEK;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre;
    contextoEjecucion->motivoDesalojo->parametros[1] = puntero;
    contextoEjecucion->motivoDesalojo->parametrosLength = 2;

    enviarContextoActualizado();
};

void f_read(char* nombre, char* direccionLogica, char* cantBytes){

    contextoEjecucion->motivoDesalojo->comando = F_READ;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre;
    contextoEjecucion->motivoDesalojo->parametros[1] = direccionLogica;
    contextoEjecucion->motivoDesalojo->parametros[2] = cantBytes;
    contextoEjecucion->motivoDesalojo->parametrosLength = 3;

    enviarContextoActualizado();
};

void f_write(char* nombre, char* direccionLogica, char* cantBytes){

    contextoEjecucion->motivoDesalojo->comando = F_WRITE;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre;
    contextoEjecucion->motivoDesalojo->parametros[1] = direccionLogica;
    contextoEjecucion->motivoDesalojo->parametros[2] = cantBytes;
    contextoEjecucion->motivoDesalojo->parametrosLength = 3;

    enviarContextoActualizado();
};

void f_truncate(char* nombre, char* tamanio){

    contextoEjecucion->motivoDesalojo->comando = F_TRUNCATE;
    contextoEjecucion->motivoDesalojo->parametros[0] = nombre;
    contextoEjecucion->motivoDesalojo->parametros[1] = tamanio;
    contextoEjecucion->motivoDesalojo->parametrosLength = 2;

    enviarContextoActualizado();
};

void create_segment(char* idSegmento, char* tamanio){

    contextoEjecucion->motivoDesalojo->comando = CREATE_SEGMENT;
    contextoEjecucion->motivoDesalojo->parametros[0] = idSegmento;
    contextoEjecucion->motivoDesalojo->parametros[1] = tamanio;
    contextoEjecucion->motivoDesalojo->parametrosLength = 2;

    enviarContextoActualizado();
};

void delete_segment(char* idSegmento){

    contextoEjecucion->motivoDesalojo->comando = DELETE_SEGMENT;
    contextoEjecucion->motivoDesalojo->parametros[0] = idSegmento; 
    contextoEjecucion->motivoDesalojo->parametrosLength = 1;

    enviarContextoActualizado();
};
/*

void mov_in(char* registro, char* direccionLogica){
};

void mov_out(char* direccionLogica, char* registro){
};  
*/
