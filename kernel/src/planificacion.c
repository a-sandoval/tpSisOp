#include "kernel/include/planificacion.h"

sem_t hayProcesosReady;
sem_t hayProcesosNuevos;
t_list* pcbsNEW; 
t_list* pcbsREADY;
int32_t procesosCreados = 0; 
pthread_mutex_t mutexListaNew; 
sem_t semGradoMultiprogramacion;
char* pidsInvolucrados; 
int64_t rafagaCPU;

int gradoMultiprogramacion; 

char* estadosProcesos[5] = {"NEW","READY","EXEC","BLOCK","SALIDA"};

// Planificacion en si

void loggearCambioDeEstado(uint32_t pid, estadoProceso anterior, estadoProceso actual) {

    log_info(logger, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pid , estadosProcesos[anterior], estadosProcesos[actual]);

}

void planificarALargoPlazo(){

    while (1) {
        sem_wait(&hayProcesosNuevos); 
        
        sem_wait(&semGradoMultiprogramacion);

        t_pcb* pcb = obtenerSiguienteAReady();
        pcb->tiempoEnReady= temporal_create(); 
        estadoProceso estadoAnterior= pcb->estado;
        pcb->estado=READY;

        loggearCambioDeEstado(pcb->pid, estadoAnterior,pcb->estado); 

        listarPIDS(pcbsREADY); 

        log_info(logger,"Cola Ready <%s>: [%s]", obtenerAlgoritmoPlanificacion(),pidsInvolucrados); 

        encolar(pcbsREADY, pcb);
        
        sem_post(&hayProcesosReady);

        
    }
}

void agregarPID(void *value) { 
	

    t_pcb* pcb = (t_pcb*) value; 
    void* id = &(pcb->pid);  
    char* pid = (char*)id; 

    string_append(&pidsInvolucrados, pid); 
    string_append(&pidsInvolucrados, " , "); 
}

void listarPIDS(t_list* pcbs) {

    list_iterate(pcbs,agregarPID);
}

void imprimirRegistros(t_dictionary *registros) {
    log_info(logger, "AX: %s", (char *) dictionary_get(registros, "AX"));
}


void planificarACortoPlazo(t_pcb* (*proximoAEjecutar)()) {

    while (1) {
        sem_wait(&hayProcesosReady); 
        t_pcb* aEjecutar = proximoAEjecutar();

        detenerYDestruirCronometro(aEjecutar->tiempoEnReady); 
        estadoProceso estadoAnterior = aEjecutar->estado;
        aEjecutar->estado = EXEC; 

        loggearCambioDeEstado(aEjecutar->pid, estadoAnterior,aEjecutar->estado);

        procesarPCB(aEjecutar);
        switch(aEjecutar->estado) {
            case READY: 
                encolar(pcbsREADY, aEjecutar);
                sem_post(&hayProcesosReady); 
                aEjecutar->rafagaRealAnterior = rafagaCPU; 
                calcularNuevaRafagaCPU(aEjecutar);
                break;
            case SALIDA:
                enviarMensaje("Terminado", aEjecutar->socketPCB);
                destruirPCB(aEjecutar);
                sem_post(&semGradoMultiprogramacion); 
                break;
            case BLOCK:
            //Luego de moverse del estado de Block a nuevamente Ready, recordar calcular rafaga con funcion calcular Rafaga. 
                break; 
            default:
                enviarMensaje("Terminado", aEjecutar->socketPCB);
                destruirPCB(aEjecutar);
                break;

        } 

        //No hacerlo por estado de proceso, sino por el mensaje que envia (por Wait, Yield (por cada syscall)). Devolver MOTIVO de devolucion, puede tener params
        
    }
}

void detenerYDestruirCronometro(t_temporal* cronometroReady) {

    temporal_stop(cronometroReady); 
    temporal_destroy(cronometroReady); 
}

void planificarACortoPlazoSegunAlgoritmo(){
    char* algoritmoPlanificador = obtenerAlgoritmoPlanificacion();

    if(!strcmp(algoritmoPlanificador,"FIFO")){
        planificarACortoPlazo(proximoAEjecutarFIFO);
    }
    else if(!strcmp(algoritmoPlanificador,"HRRN")){
        planificarACortoPlazo(proximoAEjecutarHRRN);
    }
    else{
        log_error(loggerError,"Algoritmo invalido. Debe ingresar FIFO o HRRN");
        abort();
    }
}

 t_pcb* proximoAEjecutarFIFO(){
    return desencolar(pcbsREADY);
}

void* mayorRR(void* unPCB, void* otroPCB) {

    return (calcularRR(unPCB) >=calcularRR(otroPCB)) ? unPCB : otroPCB; 

}

void calcularNuevaRafagaCPU(t_pcb* pcb) { 

    double alfa = obtenerAlfaEstimacion(); 

    pcb->estimadoRafagaAnterior = pcb->estimadoNuevaRafaga; // Lo que antes estaba como estimado de la nueva rafaga, ahora es la anterior

    double estimadoRafaga = alfa*pcb->rafagaRealAnterior + (1-alfa) * pcb->estimadoRafagaAnterior; 

    pcb->estimadoNuevaRafaga = estimadoRafaga; 

}

double calcularRR(void* elem) {

    t_pcb* pcb = (t_pcb*) elem; 

    temporal_stop(pcb->tiempoEnReady); 

    double waitTime = temporal_gettime(pcb->tiempoEnReady); 

    temporal_resume(pcb->tiempoEnReady); 

    double estimatedServiceTime = pcb->estimadoNuevaRafaga; 

    return (waitTime + estimatedServiceTime)/estimatedServiceTime; 


}

 t_pcb* proximoAEjecutarHRRN(){
    
    return list_get_maximum(pcbsREADY,mayorRR); 
    
}

// Semaforos

void inicializarSemaforos(){
    pthread_mutex_init(&mutexListaNew,NULL);
    sem_init(&hayProcesosReady,0,0);
    sem_init(&hayProcesosNuevos,0,0);
    gradoMultiprogramacion=obtenerGradoMultiprogramacion(); 
    sem_init(&semGradoMultiprogramacion,0,gradoMultiprogramacion);
}

// Operaciones con t_pcb

t_pcb* crearPCB() {

    t_pcb* nuevoPCB = malloc(sizeof(t_pcb)); 
    nuevoPCB->estado = NEW; 
    nuevoPCB->pid = procesosCreados; 
    nuevoPCB->programCounter = 0;
    nuevoPCB->instrucciones = list_create(); 
    nuevoPCB->estimadoRafagaAnterior = obtenerEstimacionInicial(); 
    nuevoPCB->tablaDeArchivos = list_create(); 
    nuevoPCB->tablaDeSegmentos=list_create(); 
    nuevoPCB->registrosCPU = crearDiccionarioDeRegistros(); 

    procesosCreados++; //para el nuevo pid

    return nuevoPCB; 

}

void destruirPCB(t_pcb* pcb){
    list_destroy_and_destroy_elements(pcb->instrucciones,(void*)destruirInstruccion); 
    list_destroy_and_destroy_elements(pcb->tablaDeArchivos,(void*)destruirInstruccion); 
    list_destroy_and_destroy_elements(pcb->tablaDeSegmentos,(void*)destruirInstruccion); 
    dictionary_destroy_and_destroy_elements(pcb->registrosCPU, (void*)destruirRegistro); 
    temporal_destroy(pcb->tiempoEnReady);
    free(pcb);
}

t_dictionary* crearDiccionarioDeRegistros() {

    t_dictionary* registros = dictionary_create(); 
    
    char *AX = malloc(sizeof(char) * (4 + 1));
    char *BX = malloc(sizeof(char) * (4 + 1));
    char *CX = malloc(sizeof(char) * (4 + 1));
    char *DX = malloc(sizeof(char) * (4 + 1));
    char *EAX = malloc(sizeof(char) * (8 + 1));
    char *EBX = malloc(sizeof(char) * (8 + 1));
    char *ECX = malloc(sizeof(char) * (8 + 1));
    char *EDX = malloc(sizeof(char) * (8 + 1));
    char *RAX = malloc(sizeof(char) * (16 + 1));
    char *RBX = malloc(sizeof(char) * (16 + 1));
    char *RCX = malloc(sizeof(char) * (16 + 1));
    char *RDX = malloc(sizeof(char) * (16 + 1));

    strncpy(AX, "0000\0", 5);
    strncpy(BX, "0000\0", 5);
    strncpy(CX, "0000\0", 5);
    strncpy(DX, "0000\0", 5);
    strncpy(EAX, "00000000\0", 9);
    strncpy(EBX, "00000000\0", 9);
    strncpy(ECX, "00000000\0", 9);
    strncpy(EDX, "00000000\0", 9);
    strncpy(RAX, "0000000000000000\0", 17);
    strncpy(RBX, "0000000000000000\0", 17);
    strncpy(RCX, "0000000000000000\0", 17);
    strncpy(RDX, "0000000000000000\0", 17);
    dictionary_put(registros, "AX",  AX); 
    dictionary_put(registros, "BX",  BX); 
    dictionary_put(registros, "CX",  CX); 
    dictionary_put(registros, "DX",  DX); 
    dictionary_put(registros, "EAX", EAX); 
    dictionary_put(registros, "EBX", EBX);
    dictionary_put(registros, "ECX", ECX);
    dictionary_put(registros, "EDX", EDX); 
    dictionary_put(registros, "RAX", RAX); 
    dictionary_put(registros, "RBX", RBX);
    dictionary_put(registros, "RCX", RCX);
    dictionary_put(registros, "RDX", RDX); 

    return registros;

}

// Lista de PCBs

void inicializarListasPCBS(){
    pcbsNEW = list_create();
    pcbsREADY = list_create();
}

void destruirListaPCB(t_list* pcbs){
    list_destroy_and_destroy_elements(pcbs,(void*)destruirPCB);
}

void ingresarANew(t_pcb *pcb) {
    pthread_mutex_lock(&mutexListaNew);
    encolar(pcbsNEW, pcb);
    log_info(logger, "Se crea el proceso <%d> en NEW", pcb->pid); 
    pthread_mutex_unlock(&mutexListaNew);
}

t_pcb *obtenerSiguienteAReady()
{
    	pthread_mutex_lock(&mutexListaNew);
    	t_pcb *pcb = desencolar(pcbsNEW);
    	pthread_mutex_unlock(&mutexListaNew);
    	return pcb;
}

void encolar(t_list* pcbs,t_pcb* pcb){
    list_add(pcbs,(void *)pcb);
}

t_pcb* desencolar(t_list* pcbs){
    return (t_pcb*)list_remove(pcbs, 0);
}

// frees

void destruirInstruccion(t_instruccion* instruccion) {
    free(instruccion); 
}

void destruirRegistro(char* registro) {
    free(registro); 
}

// Configuracion

double obtenerEstimacionInicial() {
    return config_get_double_value(config,"ESTIMACION_INICIAL"); 
}

int obtenerGradoMultiprogramacion(){
    return config_get_int_value(config,"GRADO_MAX_MULTIPROGRAMACION");
}

char* obtenerAlgoritmoPlanificacion(){
    return confGet("ALGORITMO_PLANIFICACION");
}

double obtenerAlfaEstimacion() {
    return config_get_double_value(config, "HRRN_ALFA"); 
}

void instruct_print(void *value) {
    log_info(logger, "Que linda mi instruccion: %s", (char *)value);
}

