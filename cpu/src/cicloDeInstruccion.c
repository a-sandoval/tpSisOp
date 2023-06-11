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
int tiempoEspera;

t_temporal* rafagaCPU; 
int64_t rafagaCPUEjecutada; 

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
    instruccionActual = buscar(elementosInstruccion[0], listaComandos);
}

int buscar(char *elemento, char **lista) {
    int i = 0;
    for (; strcmp(lista[i], elemento) && i <= string_array_size(lista); i++);
    return (i > string_array_size(lista)) ? -1 : i;
}
 
void execute() {

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
        /*
        case MOV_IN:
            mov_in(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case MOV_OUT:
            mov_out(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        */
        case IO:
            io(elementosInstruccion[1]);
            break;
        case F_OPEN:
            f_open(elementosInstruccion[1]);
            break;
        case F_CLOSE:
            f_close(elementosInstruccion[1]);
            break;
        case F_SEEK:
            f_seek(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case F_READ:
            f_read(elementosInstruccion[1], elementosInstruccion[2], elementosInstruccion[3]);
            break;
        case F_WRITE:
            f_write(elementosInstruccion[1], elementosInstruccion[2], elementosInstruccion[3]);
            break;
        case F_TRUNCATE:
            f_truncate(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case WAIT:
            wait_c(elementosInstruccion[1]);
            break;
        case SIGNAL:
            signal_c(elementosInstruccion[1]);
            break;
        case CREATE_SEGMENT:
            create_segment(elementosInstruccion[1], elementosInstruccion[2]);
            break;
        case DELETE_SEGMENT:
            delete_segment(elementosInstruccion[1]);
            break;
        case YIELD: 
            yield_c();
            break;
        case EXIT:
            exit_c();
            break;
        default:
            //sleep(1);
            break;
    }
}

void set_c(char* registro, char* valor){ 
    tiempoEspera = obtenerTiempoEspera();
    usleep(tiempoEspera * 1000); 
    dictionary_remove_and_destroy(contextoEjecucion->registrosCPU, registro, free); 
    dictionary_put(contextoEjecucion->registrosCPU, registro, string_duplicate(valor));
}

void io(char* tiempo){
    destruirTemporizador(rafagaCPU);
    modificarMotivoDesalojo (IO, 1, tiempo, "", "");
    enviarContextoActualizado(socketCliente);
}

void wait_c(char* recurso){
    modificarMotivoDesalojo (WAIT, 1, recurso, "", "");
    enviarContextoActualizado(socketCliente);
}

void signal_c(char* recurso){
    modificarMotivoDesalojo (SIGNAL, 1, recurso, "", "");
    enviarContextoActualizado(socketCliente);
}

void yield_c(){ 
    destruirTemporizador(rafagaCPU);
    modificarMotivoDesalojo (YIELD, 0, "", "", "");
    enviarContextoActualizado(socketCliente);
}

void exit_c () {
    destruirTemporizador(rafagaCPU);
    char * terminado = string_duplicate ("SUCCESS");
    modificarMotivoDesalojo (EXIT, 1, terminado, "", "");
    enviarContextoActualizado(socketCliente);
    free (terminado);
}

void f_open(char* nombre){
    modificarMotivoDesalojo (F_OPEN, 1, nombre, "", "");
    enviarContextoActualizado(socketCliente);
};

void f_close(char* nombre){
    modificarMotivoDesalojo (F_CLOSE, 1, nombre, "", "");
    enviarContextoActualizado(socketCliente);
};

void f_seek(char* nombre, char* puntero){
    modificarMotivoDesalojo (F_SEEK, 2, nombre, puntero, "");
    enviarContextoActualizado(socketCliente);
};

void f_read(char* nombre, char* direccionLogica, char* cantBytes){
    int dirFisica = mmu (direccionLogica);
    char* direccionFisica = string_itoa(dirFisica);
    modificarMotivoDesalojo (F_READ, 3, nombre, direccionFisica, cantBytes);
    enviarContextoActualizado(socketCliente);
};

void f_write(char* nombre, char* direccionLogica, char* cantBytes){
    int dirFisica = mmu(direccionLogica);
    char* direccionFisica = string_itoa(dirFisica);
    modificarMotivoDesalojo (F_WRITE, 3, nombre, direccionFisica, cantBytes);
    enviarContextoActualizado(socketCliente);
};

void f_truncate(char* nombre, char* tamanio){
    modificarMotivoDesalojo (F_TRUNCATE, 2, nombre, tamanio, "");
    enviarContextoActualizado(socketCliente);
};

void create_segment(char* idSegmento, char* tamanio){
    modificarMotivoDesalojo (CREATE_SEGMENT, 2, idSegmento, tamanio, "");
    enviarContextoActualizado(socketCliente);
};

void delete_segment(char* idSegmento){
    modificarMotivoDesalojo (DELETE_SEGMENT, 1, idSegmento, "", "");
    enviarContextoActualizado(socketCliente);
};

void destruirTemporizador (t_temporal * temporizador) {
    temporal_stop(temporizador);
    contextoEjecucion->rafagaCPUEjecutada = temporal_gettime(temporizador);  
    //temporal_destroy(temporizador); 
}

void modificarMotivoDesalojo (t_comando comando, int numParametros, char * parm1, char * parm2, char * parm3) {
    char * (parametros[3]) = { parm1, parm2, parm3 };
    contextoEjecucion->motivoDesalojo->comando = comando;
    for (int i = 0; i < numParametros; i++)
        contextoEjecucion->motivoDesalojo->parametros[i] = string_duplicate(parametros[i]);
    contextoEjecucion->motivoDesalojo->parametrosLength = numParametros;
}

void liberarMemoria() {
    for (int i = 0; i <= cantParametros; i++) free(elementosInstruccion[i]);
    free(elementosInstruccion);
}


// RECORDAR LOG MINIMO POR SEG FAULT 
void mov_in(char* registro, char* direccionLogica){

    int valor;
    int dirFisica = mmu(direccionLogica);

    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = READ;
    agregarAPaquete(peticion,(void*)&dirFisica, sizeof(int));
    enviarPaquete(peticion, conexionAMemoria);

    valor = recibirValor(conexionAMemoria);

    //chequeoDesplazamiento(direccionLogica, valor);

    // recien aca podria chequear que lo que leyó mas el nro de desplazamiento no haya sido mayor al tamaño del segmento

    char* valorInsertar=string_itoa(valor);
    dictionary_remove_and_destroy(contextoEjecucion->registrosCPU, registro, free); 
    dictionary_put(contextoEjecucion->registrosCPU, registro, string_duplicate(valorInsertar));

    //log_info(logger, "PID: %d - Accion: %s -  Segmento: %d - Direccion Fisica: %d - Valor:  %d", contextoEjecucion->pid, "LEER", nroSegmento, dirFisica, valor );
    

};

void mov_out(char* direccionLogica, char* registro){

    void* valor = dictionary_get(contextoEjecucion->registrosCPU, registro);

    //chequeoDesplazamiento(direccionLogica, valor);
    int dirFisica = mmu(direccionLogica);

    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = WRITE;

    agregarAPaquete(peticion, (void*)&dirFisica, sizeof(int));
    agregarAPaquete(peticion, valor, sizeof(char) * 16); // como mucho es un registro de 16 bytes no?

    enviarPaquete(peticion, conexionAMemoria);

    recibirMensaje(conexionAMemoria);
    // que hago cuando recibo la confimracion?

    //log_info(logger, "PID: %d - Accion: %s -  Segmento: %d - Direccion Fisica: %d - Valor:  %d", contextoEjecucion->pid, "WRITE", nroSegmento, dirFisica, valor);
};  


int mmu(char* direccionLogica){
    int dirFisica;
    int dirLogica = direccionLogica - '0';
    int tamMaxSegmento = obtenerTamanioMaxSeg();

    int numSegmento = floor(dirLogica/tamMaxSegmento);
    int desplazamiento = dirLogica % tamMaxSegmento;

    t_segmento* segmento = list_get(contextoEjecucion->tablaDeSegmentos, numSegmento);
    
    int base = segmento->direccionBase;
    
    dirFisica = base + desplazamiento;
    return dirFisica;

    
}

int recibirValor(int socket) {
    int valor;
	int size, desplazamiento = 0;
	void * buffer;

	buffer = recibirBuffer(socket, &size);

    desplazamiento += sizeof(int);
    memcpy(&(valor), buffer, sizeof(int));

	free(buffer);

    return valor;
}


