#include "cpu/include/cicloDeInstruccion.h"

void cicloDeInstruccion(){
    fetch();

    decode();

    executeCPU();
}
 
void executeCPU(){
        //SET (Registro, Valor) --> Asigna al registro el valor pasado como parámetro.
        set(t_reg registro, int valor){
            int tiempoEspera = obtenerTiempoEspera();
            sleep(tiempoEspera);
            registro.registro1 = valor;
        }

        obtenerTiempoEspera(){
            return confGet("RETARDO_INSTRUCCION");
        }
        //YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.
        

    
        //EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.
    }
// IR ACTUALIZANDO EL PCB MIENTRAS SE VA EJECUTANDO
