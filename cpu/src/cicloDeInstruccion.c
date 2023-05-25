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
char* direccionLogica;
int tiempo;//miedo
char* nombreArchivo;
uint32_t posicion;
int cantBytes;
int tamanio;
char* recurso;
uint32_t idSegmento;
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

   rafagaCPU= temporal_create(); 

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
            set_c(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case MOV_IN://falta
            mov_in(elementosInstruccion[1], direccionLogica);
            break;
        case MOV_OUT://falta
            mov_out(direccionLogica, elementosInstruccion[1]);
            break;
        case IO://IMPLEMENTAR
            io(tiempo);
            break;
        case F_OPEN://falta
            f_open(nombreArchivo);
            break;
        case F_CLOSE://falta
            f_close(nombreArchivo);
            break;
        case F_SEEK://falta
            f_seek(nombreArchivo, posicion);
            break;
        case F_READ://falta
            f_read(nombreArchivo, direccionLogica, cantBytes);
            break;
        case F_WRITE://falta
            f_write(nombreArchivo, direccionLogica, cantBytes);
            break;
        case F_TRUNCATE://falta
            f_truncate(nombreArchivo, tamanio);
            break;
        case WAIT://IMPLEMENTAR
            wait_c(recurso);
            //un recurso puede ser un archivo, memoria reservada, semáforos, sockets, etc
            break;
        case SIGNAL://IMPLEMENTAR
            signal_c(recurso);
            break;
        case CREATE_SEGMENT://falta
            create_segment(idSegmento, tamanio);
            break;
        case DELETE_SEGMENT://falta
            delete_segment(idSegmento);
            break;
        case YIELD: //IMPLEMENTADA
            yield_c();
            break;
        case EXIT: //IMPLEMENTADA
            exit_c();
            break;
        default:
            //sleep(1);
            break;
    }
}

// ------- Funciones del execute SET - YIELD - EXIT ------- //

//SET (Registro, Valor) --> Asigna al registro el valor pasado como parámetro.
void set_c(char* registro, char* valor){
    int tiempoEspera = obtenerTiempoEspera();
    sleep(tiempoEspera); 
    dictionary_remove_and_destroy(contextoEjecucion->registrosCPU, registro, free); 
    dictionary_put(contextoEjecucion->registrosCPU, registro, string_duplicate(valor));
}

int obtenerTiempoEspera(){
    return config_get_int_value(config,"RETARDO_INSTRUCCION"); 
}

//IO (Tiempo) --> Representa una syscall de I/O bloqueante. Devuelve el Contexto de Ejecución actualizado al Kernel junto a la cantidad de unidades de tiempo que va a bloquearse el proceso.
void io(int tiempo){
    temporal_stop(rafagaCPU); 
    
    rafagaCPUEjecutada = temporal_gettime(rafagaCPU);  

    enviarContextoActualizado();
    //enviarCantidadDeTiempoABloquearse(cantidad);
}

//WAIT (Recurso) --> Solicita al Kernel que se asigne una instancia del recurso indicado por parámetro.
void wait_c(char* recurso){
    
    //Si hay un bloqueo, hay que detener la rafaga de CPU
    //temporal_stop(rafagaCPU); 
    enviarContextoActualizado();
    solicitarInstancia(recurso);
}

void solicitarInstancia(char* recurso){
    //flor está en proceso de pensar esto
}

//SIGNAL (Recurso) --> Solicita al Kernel que se libere una instancia del recurso indicado por parámetro.
void signal_c(char* recurso){

    //Si hay un bloqueo, hay que detener la rafaga de CPU

    //temporal_stop(rafagaCPU); 
    enviarContextoActualizado();
    liberarInstancia(recurso);
}

void liberarInstancia(char* recurso){
    //flor está en proceso de pensar esto tmb
}

//YIELD --> Desaloja voluntariamente el proceso de la CPU. Devuelve el Contexto de Ejecución actualizado al Kernel.
void yield_c(){ 
    contextoEjecucion->estado = READY;
    temporal_stop(rafagaCPU);
    int64_t rafagaCPUEjecutada = temporal_gettime(rafagaCPU);  
    enviarContextoActualizado();
}

//EXIT --> Representa la syscall de finalización del proceso. Devuelve el Contexto de Ejecución actualizado al Kernel para su finalización.
void exit_c(){
    contextoEjecucion->estado = SALIDA;
    temporal_stop(rafagaCPU); 
    temporal_destroy(rafagaCPU); 
    enviarContextoActualizado();
}

void enviarContextoActualizado(){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
   
    // cargo todos los valores en el paquete
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->estado, sizeof(estadoProceso));

    //agregarInstruccionesAPaquete(paquete, contextoEjecucion->instrucciones);

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
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * (strlen(list_get(instrucciones, i)) + 1 ));
    }
}

void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU){
     
     // no terminaran en /0???
    char *AX = dictionary_get(registrosCPU,"AX"); 
    agregarAPaquete(paquete, AX, sizeof(char) * (4 + 1));
    //free(AX);

    char* BX = dictionary_get(registrosCPU,"BX");
    agregarAPaquete(paquete, BX, sizeof(char) * (4 + 1));
    //free(BX);

    char* CX = dictionary_get(registrosCPU,"CX");
    agregarAPaquete(paquete, CX, sizeof(char) * (4 + 1));
    //free(CX);

    char* DX = dictionary_get(registrosCPU,"DX");
    agregarAPaquete(paquete, DX, sizeof(char) * (4 + 1));
    //free(DX);

    char* EAX = dictionary_get(registrosCPU,"EAX");
    agregarAPaquete(paquete, EAX, sizeof(char) * (8 + 1));
    //free(EAX);

    char* EBX = dictionary_get(registrosCPU,"EBX");
    agregarAPaquete(paquete, EBX, sizeof(char) * (8 + 1));
    //free(EBX);

    char* ECX = dictionary_get(registrosCPU,"ECX");
    agregarAPaquete(paquete, ECX, sizeof(char) * (8 + 1));
    //free(ECX);

    char* EDX = dictionary_get(registrosCPU,"EDX");
    agregarAPaquete(paquete, EDX, sizeof(char) * (8 + 1));
    //free(EDX);

    char* RAX = dictionary_get(registrosCPU,"RAX");
    agregarAPaquete(paquete, RAX, sizeof(char) * (16 + 1));
    //free(RAX);

    char* RBX = dictionary_get(registrosCPU,"RBX");
    agregarAPaquete(paquete, RBX, sizeof(char) * (16 + 1));
    //free(RBX);

    char* RCX = dictionary_get(registrosCPU,"RCX");
    agregarAPaquete(paquete, RCX, sizeof(char) * (16 + 1));
    //free(RCX);

    char* RDX = dictionary_get(registrosCPU,"RDX");
    agregarAPaquete(paquete, RDX, sizeof(char) * (16 + 1));
    //free(RDX);
}

int buscar(char *elemento, char **lista) {
    int i = 0;
    for (; strcmp(lista[i], elemento) && i <= string_array_size(lista); i++);
    return (i > string_array_size(lista)) ? -1 : i;
}

//A continuación serán declaradas las operaciones que faltan, falta desarrollarlas 
//pero serán puestas para que la consola no nos hinche los huevos
void mov_in(char* registro, char* direccionLogica){
};

void mov_out(char* direccionLogica, char* registro){
};  

void f_open(char* nombre){
};

void f_close(char* nombre){
};

void f_seek(char* nombre, uint32_t puntero){
};

void f_read(char* nombre, char* direccionLogica, int cantBytes){
};

void f_write(char* nombre, char* direccionLogica, int cantBytes){
};

void f_truncate(char* nombre, int tamanio){
};

void create_segment(uint32_t idSegmento, int tamanio){
};

void delete_segment(uint32_t idSegmento){
};