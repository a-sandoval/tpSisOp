#include "cpu/include/cicloDeInstruccion.h"

char* instruccionAEjecutar; 
char** elementosInstruccion; 
int instruccionActual; 
int cantParametros;
t_contexto* contextoActual; 

void cicloDeInstruccion(t_contexto* contextoRecibido){

    contextoActual = contextoRecibido; 
    
    fetch();//busca la próxima instruccion a ejecutar. Lista en pcb

    decode();//interpreta que instruccion va a ejecutar y si requiere traduccion logica o fisica

    execute();//ejecuta la instruccion 
}

// ------- Funciones del ciclo ------- //
void fetch() { 
    int numInstruccionABuscar = contextoActual->programCounter;
    instruccionAEjecutar = list_get(contextoActual->instrucciones,numInstruccionABuscar); 
    contextoActual->programCounter=+1;
}

void decode(){
    instruccionAEjecutar = string_replace(instruccionAEjecutar, "\0", "");
    elementosInstruccion = string_split(instruccionAEjecutar, " ");
    cantParametros = string_array_size(elementosInstruccion) - 1;
    instruccionActual = buscar(elementosInstruccion[0], listaComandos);
}
 
void execute() {
    switch(cantParametros) {
        case 0:
            log_info(logger, "PID: %d - Ejecutando: %s ", contextoActual->pid, elementosInstruccion[0]);
            break;
        case 1:
            log_info(logger, "PID: %d - Ejecutando: %s -  %s", contextoActual->pid, elementosInstruccion[0], elementosInstruccion[1]);
            break;
        case 2:   
            log_info(logger, "PID: %d - Ejecutando: %s -  %s, %s", contextoActual->pid, elementosInstruccion[0], elementosInstruccion[1], elementosInstruccion[2]);
            break; 
        case 3:
            log_info(logger, "PID: %d - Ejecutando: %s -  %s, %s, %s", contextoActual->pid, elementosInstruccion[0], elementosInstruccion[1], elementosInstruccion[2], elementosInstruccion[3]);
            break; 
    }
    switch(instruccionActual){
        case SET:
            set_c(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case YIELD:
            yield_c();
            break;
        case EXIT:
            exit_c();
            break;
        default:
            break;
    }
}

// ------- Funciones del execute SET - YIELD - EXIT ------- //

//SET (Registro, Valor) --> Asigna al registro el valor pasado como parámetro.
void set_c(char* registro, char* valor){
    int tiempoEspera = obtenerTiempoEspera();
    sleep(tiempoEspera);
    dictionary_put(contextoActual->registrosCPU, registro, valor);
}

int obtenerTiempoEspera(){
    return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

//YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.
void yield_c(){ 
    contextoActual->estado = READY;
    enviarContextoActualizado();
}

//EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.
void exit_c(){
    contextoActual->estado = SALIDA;
    enviarContextoActualizado();
}

//Funciones necesarias
void enviarContextoActualizado(){
    // serializar_contextoEjecucion(t_paquete* paquete, int bytes);
    enviar_contexto();
}

int buscar(char *elemento, char **lista) {
    int i = 0;
    for (; strcmp(lista[i], elemento) && i <= string_array_size(lista); i++);
    return (i > string_array_size(lista)) ? -1 : i;
}