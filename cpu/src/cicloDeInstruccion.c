#include "cpu/include/cicloDeInstruccion.h"

t_contexto *contextoActual;

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
void set_c(char* registro, char* valor){
    int tiempoEspera = obtenerTiempoEspera();
    sleep(tiempoEspera);
    strcpy(registro, valor);
}

int obtenerTiempoEspera(){
    return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

int process_getpid() {
    return contextoActual->pid;
}

void enviarContexto(int procesoID){
    //serializar
}

//YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.

void yield_c(){ 
    uint32_t proceso = process_getpid();
    //PCB -> estado = READY; //Asumo que está en EXEC el proceso
    enviarContexto(proceso);
}

//EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.

void exit_c(){
    int32_t proceso = process_getpid();
    //PCB -> estado = SALIDA;
    enviarContexto(proceso);
}