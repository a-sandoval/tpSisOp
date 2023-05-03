#include "cpu/include/cicloDeInstruccion.h"

void cicloDeInstruccion(t_contexto *contextoActual){
    fetch();//busca la próxima instruccion a ejecutar. Lista en pcb

    decode();//interpreta que instruccion va a ajecutar y si requiere traduccion logica o fisica

    execute();//ejecuta la instruccion
}

// ------- Funciones del ciclo ------- //
void fetch() {
    /* Funcion choreada de Fede que tengo que moldear:
    t_comando buscarComando(char *comando) {
        int i = 0;
        while(listaComandos[i].cantParametros != -1 && !string_contains(comando, listaComandos[i].nombre)) i++;
        return listaComandos[i];
    }
    */
}

void decode(){

}
 
void execute(){
    //case de instrucciones, dependiendo del caso se mete en una y ejecuta
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