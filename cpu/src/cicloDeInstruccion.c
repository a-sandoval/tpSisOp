#include "cpu/include/cicloDeInstruccion.h"

char* instruccionAEjecutar; 
char** elementosInstruccion; 
int instruccionActual; 
int cantParametros;
t_contexto* contextoEjecucion; 

void cicloDeInstruccion(t_contexto* contextoRecibido){

    contextoEjecucion = contextoRecibido; 
    
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

//Funciones necesarias

void enviar_contexto(){ 
    t_paquete* paquete = malloc(sizeof(t_paquete));
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
	paquete->buffer = malloc(sizeof(t_buffer));
   
    // cargo todos los valores en el paquete
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    //agregarAPaquete(paquete,&contextoEjecucion->registrosCPU, sizeof(contextoEjecucion->registrosCPU)); // a chequear ese ampersand

    agregarListaAPaquete(paquete, contextoEjecucion->instrucciones);

    //hay que ver si dejamos el dictionary, si es asi se serializa como las instrucciones porque tmb seria de char*
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->registrosSize, sizeof(contextoEjecucion->registrosSize));
    //agregarAPaquete(paquete,contextoEjecucion->registrosCPU, contextoEjecucion->registrosSize);

    //no sabemos listas de que son estas tablas entonces aun no podemos serializar
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeArchivosSize, sizeof(contextoEjecucion->tablaDeArchivosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeArchivos, contextoEjecucion->tablaDeArchivosSize);
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeSegmentosSize, sizeof(contextoEjecucion->tablaDeSegmentosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeSegmentos, contextoEjecucion->tablaDeSegmentosSize);
  
    agregarAPaquete(paquete,(void *)contextoEjecucion->estado, sizeof(estadoProceso));

    enviarPaquete(paquete,socketCliente);

	eliminarPaquete(paquete);
}

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones){
    //necesitaria agregar un /0 al final de cada instruccion

    contextoEjecucion->instruccionesLength = list_size(instrucciones);
    
    agregarAPaquete(paquete,(void*)&contextoEjecucion->instruccionesLength, sizeof(uint32_t)); //primero envio la cantidad de elementos
    uint32_t i;
    for(i=0;i<contextoEjecucion->instruccionesLength;i++){
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * strlen(list_get(instrucciones, i)) + 1 );
    }
}


void enviarContextoActualizado(){
    // serializar_contextoEjecucion(t_paquete* paquete, int bytes);
    enviar_contexto();
}

int buscar(char *elemento, char **lista) {
    int i = 0;
    for (; strcmp(lista[i], elemento) && i <= string_array_size(lista); i++);
    return (i > string_array_size(lista)) ? -1 : i;
}