#include "cpu/include/cicloDeInstruccion.h"


void cicloDeInstruccion(){
    fetch();

    decode();

    executeCPU();
}
// IR ACTUALIZANDO EL PCB MIENTRAS SE VA EJECUTANDO 

 
void executeCPU(){
}

//SET (Registro, Valor) --> Asigna al registro el valor pasado como parámetro.
void set(t_reg registro, int valor){
            
            int tiempoEspera = obtenerTiempoEspera();
            sleep(tiempoEspera);
            registro.registro1 = valor;
    
    }

int obtenerTiempoEspera(){
            return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

//YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.

void yield(){ //No estpy segura del tipo de dato que es un proceso
    int32_t proceso = process_getpid();
    free(proceso); //Revisar commons si hay algo que flete un proceso
    devolverContextoActualizado(proceso);
}  

devolverContextoActualizado(proceso){
    //serializar
    //
}

//EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.

void exit(){
    int32_t proceso = process_getpid();
    // estadoProceso.EXI --> A quien iría atribuído?
    devolverContextoActualizado(proceso);
}