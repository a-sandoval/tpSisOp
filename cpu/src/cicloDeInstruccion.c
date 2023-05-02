#include "cpu/include/cicloDeInstruccion.h"

void cicloDeInstruccion(){
    // IR ACTUALIZANDO EL PCB MIENTRAS SE VA EJECUTANDO 
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

// ------- Funciones del exceute SET - YIELD - EXIT ------- //

//SET (Registro, Valor) --> Asigna al registro el valor pasado como parámetro.
void set(char* registro, char* valor){
    int tiempoEspera = obtenerTiempoEspera();
    sleep(tiempoEspera);
    strcpy(registro, valor);
}

int obtenerTiempoEspera(){
    return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

//YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.

void yield(){ 
    int32_t proceso = pocess_getpid();
    //PCB -> estado = READY; //Asumo que está en EXEC el proceso
    enviarContexto(proceso);
}

enviarContexto(procesoID){
    //serializar
}

//EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.

void exit(){
    int32_t proceso = process_getpid();
    //PCB -> estado = SALIDA;
    enviarContexto(proceso);
}