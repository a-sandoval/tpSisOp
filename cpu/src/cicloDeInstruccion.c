#include "cpu/include/cicloDeInstruccion.h"

void cicloDeInstruccion(t_contexto* contextoActual){
    int instruccionAEjecutar;
    void fetch(instruccionAEjecutar);//busca la próxima instruccion a ejecutar. Lista en pcb

    decode();//interpreta que instruccion va a ajecutar y si requiere traduccion logica o fisica

    execute(instruccionAEjecutar);//ejecuta la instruccion
}

// ------- Funciones del ciclo ------- //
void fetch(instruccionAEjecutar) { // Esta parte grita REVISAR
    t_list* listaInstrucciones = contextoActual->instrucciones;
    int numInstruccionABuscar = contextoActual->programCounter;
    instruccionAEjecutar = listaComandos[numInstruccionABuscar];
    contextoActual->programCounter=+1;
}

void decode(){

}
 
void execute(instruccionAEjecutar){
    while(1){
        switch(instruccionAEjecutar){
            case SET:
                char* registro, valor; //?
                set_c(registro, valor);
                break;
            case YIELD:
                yield_c();
                break;
            case EXIT:
                exit_c();
                break;
        }
    }
}

// ------- Funciones del execute SET - YIELD - EXIT ------- //

//SET (Registro, Valor) --> Asigna al registro el valor pasado como parámetro.
void set_c(char* registro, char* valor){
    int tiempoEspera = obtenerTiempoEspera();
    sleep(tiempoEspera);
    strcpy(registro, valor);
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