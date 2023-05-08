#include "cpu/include/cicloDeInstruccion.h"

t_contexto* contextoEjecucion;

void cicloDeInstruccion(){

    fetch();//busca la próxima instruccion a ejecutar. Lista en pcb

    decode();//interpreta que instruccion va a ejecutar y si requiere traduccion logica o fisica

    execute();//ejecuta la instruccion 
}

// ------- Funciones del ciclo ------- //
void fetch() { 
    int numInstruccionABuscar = contextoEjecucion->programCounter;
    instruccionAEjecutar = list_get(contextoEjecucion->instrucciones,numInstruccionABuscar); 
    contextoEjecucion->programCounter=+1;
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
            log_info(logger, "PID: %d - Ejecutando: %s ", contextoEjecucion->pid, elementosInstruccion[0]);
            break;
        case 1:
            log_info(logger, "PID: %d - Ejecutando: %s -  %s", contextoEjecucion->pid, elementosInstruccion[0], elementosInstruccion[1]);
            break;
        case 2:   
            log_info(logger, "PID: %d - Ejecutando: %s -  %s, %s", contextoEjecucion->pid, elementosInstruccion[0], elementosInstruccion[1], elementosInstruccion[2]);
            break; 
        case 3:
            log_info(logger, "PID: %d - Ejecutando: %s -  %s, %s, %s", contextoEjecucion->pid, elementosInstruccion[0], elementosInstruccion[1], elementosInstruccion[2], elementosInstruccion[3]);
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
    free(dictionary_get(contextoEjecucion->registrosCPU,registro)); 
    dictionary_put(contextoEjecucion->registrosCPU, registro, valor);
}

int obtenerTiempoEspera(){
    return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

//YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.
void yield_c(){ 
    contextoEjecucion->estado = READY;
    enviarContextoActualizado();
}

//EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.
void exit_c(){
    contextoEjecucion->estado = SALIDA;
    enviarContextoActualizado();
}

void enviarCOntextoActualizado(){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
   
    // cargo todos los valores en el paquete
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->estado, sizeof(estadoProceso));


    agregarRegistrosAPaquete(paquete, contextoEjecucion->registrosCPU);

    //no sabemos listas de que son estas tablas entonces aun no podemos serializar o hay que serializarlo como listas y ver si dsps cambia
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeArchivosSize, sizeof(contextoEjecucion->tablaDeArchivosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeArchivos, contextoEjecucion->tablaDeArchivosSize);
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeSegmentosSize, sizeof(contextoEjecucion->tablaDeSegmentosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeSegmentos, contextoEjecucion->tablaDeSegmentosSize);
    
    enviarPaquete(paquete, socketCliente);

	eliminarPaquete(paquete);
}

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones){

    contextoEjecucion->instruccionesLength = list_size(instrucciones);
    
    agregarAPaquete(paquete, &contextoEjecucion->instruccionesLength, sizeof(uint32_t)); //primero envio la cantidad de elementos
    uint32_t i;
    for(i=0;i<contextoEjecucion->instruccionesLength;i++){
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * strlen(list_get(instrucciones, i)) + 1 );
    }
}

void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU){
     
     // no terminaran en /0???
    char* AX = dictionary_get(registrosCPU,"AX"); 
    agregarAPaquete(paquete, AX, sizeof(char) * 4);
    free(AX);

    char* BX = dictionary_get(registrosCPU,"BX");
    agregarAPaquete(paquete, BX, sizeof(char) * 4 );
    free(BX);

    char* CX = dictionary_get(registrosCPU,"CX");
    agregarAPaquete(paquete, CX, sizeof(char) * 4 );
    free(CX);

    char* DX = dictionary_get(registrosCPU,"DX");
    agregarAPaquete(paquete, DX, sizeof(char) * 4 );
    free(DX);

    char* EAX = dictionary_get(registrosCPU,"EAX");
    agregarAPaquete(paquete, EAX, sizeof(char) * 8 );
    free(EAX);

    char* EBX = dictionary_get(registrosCPU,"EBX");
    agregarAPaquete(paquete, EBX, sizeof(char) * 8 );
    free(EBX);

    char* ECX = dictionary_get(registrosCPU,"ECX");
    agregarAPaquete(paquete, ECX, sizeof(char) * 8 );
    free(ECX);

    char* EDX = dictionary_get(registrosCPU,"EDX");
    agregarAPaquete(paquete, EDX, sizeof(char) * 8 );
    free(EDX);

    char* RAX = dictionary_get(registrosCPU,"RAX");
    agregarAPaquete(paquete, RAX, sizeof(char) * 16 );
    free(RAX);

    char* RBX = dictionary_get(registrosCPU,"RBX");
    agregarAPaquete(paquete, RBX, sizeof(char) * 16 );
    free(RBX);

    char* RCX = dictionary_get(registrosCPU,"RCX");
    agregarAPaquete(paquete, RCX, sizeof(char) * 16 );
    free(RCX);

    char* RDX = dictionary_get(registrosCPU,"RDX");
    agregarAPaquete(paquete, RDX, sizeof(char) * 16 );
    free(RDX);
}



int buscar(char *elemento, char **lista) {
    int i = 0;
    for (; strcmp(lista[i], elemento) && i <= string_array_size(lista); i++);
    return (i > string_array_size(lista)) ? -1 : i;
}