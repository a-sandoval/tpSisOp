/* KERNEL- cliente | CPU - servidor*/
#include "kernel/include/conexionCPU.h"


t_buffer bufferContexto;
t_contexto* contextoEjecucion;
int conexionACPU;

void conexionCPU() {
    while(1) {
        conexionACPU = conexion("CPU");
        if(conexionACPU != -1)
            return;
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionACPU);
            sleep(5);
        }
    }
}

int procesarPCB(t_pcb* procesoEnEjecucion) {

    contextoEjecucion = malloc(sizeof(t_contexto));

    logger = cambiarNombre("Kernel-CPU");
    asignarPCBAContexto(procesoEnEjecucion);
    enviar_contexto();

    // aca a su vez hay que recibir el contexto actualizado que mande la cpu, deserializarlo y cambiarlo en el PCB
    //Noc si esto iria aca porque en realidad seria como un case compartido con las cosas
    //que tiene que recibir de memoria y fs tm no?
    /*
    int operacion=recibir_operacion();

    switch(operacion){
        case CONTEXTOEJECUCION:
            recibir_contexto(); //me carga el contexto actualizado en el mismo contextoEjecucion;
            actualizarPCB(procesoEnEjecucion);
    }
*/
    //close(conexionACPU);
    
    return 0;
 
}

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION

void asignarPCBAContexto(t_pcb*  proceso){

    contextoEjecucion->estado = proceso->estado;
    contextoEjecucion->instrucciones = proceso->instrucciones;
    contextoEjecucion->pid = proceso->pid;
    contextoEjecucion->programCounter = proceso->programCounter;
    contextoEjecucion->registrosCPU = proceso->registrosCPU;
    contextoEjecucion->tablaDeArchivos = proceso->tablaDeArchivos;
    contextoEjecucion->tablaDeSegmentos = proceso->tablaDeSegmentos;
    contextoEjecucion->instruccionesLength = 0;
}

void enviar_contexto(){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
	//paquete->buffer = malloc(sizeof(t_buffer));
   
    // cargo todos los valores en el paquete
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->estado, sizeof(estadoProceso));

    //agregarAPaquete(paquete,&contextoEjecucion->registrosCPU, sizeof(contextoEjecucion->registrosCPU)); // a chequear ese ampersand

    agregarInstruccionesAPaquete(paquete, contextoEjecucion->instrucciones);

    //hay que ver si dejamos el dictionary, si es asi se serializa como las instrucciones porque tmb seria de char*
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->registrosSize, sizeof(contextoEjecucion->registrosSize));
    //agregarAPaquete(paquete,contextoEjecucion->registrosCPU, contextoEjecucion->registrosSize);

    //no sabemos listas de que son estas tablas entonces aun no podemos serializar
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeArchivosSize, sizeof(contextoEjecucion->tablaDeArchivosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeArchivos, contextoEjecucion->tablaDeArchivosSize);
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeSegmentosSize, sizeof(contextoEjecucion->tablaDeSegmentosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeSegmentos, contextoEjecucion->tablaDeSegmentosSize);
  
    

    enviarPaquete(paquete, conexionACPU);

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



//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU
void recibir_contexto(){
    //deserializar

	int size;
	int desplazamiento = 0;
	void * buffer;


	buffer = recibir_buffer(&size);
	while(desplazamiento < size){ //segun entiendo el while hace que se quede esperando a recbibir absoulamente todos los datos
        memcpy(&(contextoEjecucion->pid), buffer + desplazamiento, sizeof(contextoEjecucion->pid));
        desplazamiento+=sizeof(contextoEjecucion->pid);
        memcpy(&(contextoEjecucion->programCounter), buffer+desplazamiento, sizeof(contextoEjecucion->programCounter));
        desplazamiento+=sizeof(contextoEjecucion->programCounter);
        memcpy(&(contextoEjecucion->estado), buffer+desplazamiento, sizeof(estadoProceso));
        desplazamiento+=sizeof(estadoProceso);
        memcpy(&(contextoEjecucion->registrosCPU), buffer+desplazamiento, sizeof(contextoEjecucion->registrosCPU));
        desplazamiento+=sizeof(contextoEjecucion->registrosCPU);
    
        memcpy(&(contextoEjecucion->instruccionesLength), buffer+desplazamiento, sizeof(contextoEjecucion->instruccionesLength));
        desplazamiento += sizeof(contextoEjecucion->instruccionesLength);
        contextoEjecucion->instrucciones = malloc(contextoEjecucion->instruccionesLength);
        memcpy(contextoEjecucion->instrucciones , buffer+desplazamiento, contextoEjecucion->instruccionesLength);
		
       
		
	}

	free(buffer);

}

void actualizarPCB(t_pcb* proceso){
    proceso->estado = contextoEjecucion->estado;
    proceso->instrucciones = contextoEjecucion->instrucciones;
    proceso->pid = contextoEjecucion->pid;
    proceso->programCounter = contextoEjecucion->programCounter;
    proceso->registrosCPU = contextoEjecucion->registrosCPU;
    proceso->tablaDeArchivos = contextoEjecucion->tablaDeArchivos;
    proceso->tablaDeSegmentos = contextoEjecucion->tablaDeSegmentos;

}
