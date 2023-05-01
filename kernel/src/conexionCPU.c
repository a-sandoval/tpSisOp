/* KERNEL- cliente | CPU - sevidor*/
#include "kernel/include/conexionCPU.h"


t_buffer bufferContexto;
t_contexto* contextoEjecucion;

int conexionCPU() {

    logger = cambiarNombre("Kernel-CPU");
    int conexionACPU = conexion("CPU");
    if(!(conexionACPU + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else{
        // aca tendria que asignarle al pcb global los valores que le corresponden al proximo proceso que va
        // de ready a exec, para pasarselo a la cpu
        //enviar_contexto();

        // aca a su vez hay que recibir el contexto actualizado que mande la cpu, deserializarlo y cambiarlo en el PCB
        //Noc si esto iria aca porque en realidad seria como un case compartido con las cosas
        //que tiene que recibir de memoria y fs tm no?
        int operacion=recibir_operacion();

        switch(operacion){
            case CONTEXTOEJECUCION:
                recibir_contexto();
        }
        close(conexionACPU);

    }
    
    return 0;
 
}

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION

void* serializar_contextoEjecucion(t_paquete* paquete, int bytes){ //creo que quedo igual al serializar paquete original
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

//lo de las funciones crear paquete y crear buffer se esta haciendo aca adentro 
//enviar contexto podria ser una llamada a crear paquete y agregar a paquete enviar paquete y eliminar
void enviar_contexto(){ //socket es global y el pcb de donde saco el contexto tmb
    t_paquete* paquete = malloc(sizeof(t_paquete));
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
	paquete->buffer = malloc(sizeof(t_buffer));
    /* esto lo hace tmb la funcion agregar a paquete
        indique el tamaño del buffer y pedi el lugar necesario para poder cargar el stream con los valores a enviar

    paquete->buffer->size = sizeof(contextoEjecucion->pid)+sizeof(contextoEjecucion->socketPCB)+
                            sizeof(contextoEjecucion->programCounter)+sizeof(contextoEjecucion->registrosCPU)+
                            sizeof(contextoEjecucion->instruccionesLength)+strlen(contextoEjecucion->instrucciones); //noc si funciona esto de las instrucciones
    paquete->buffer->stream = malloc(paquete->buffer->size);
    */
    // cargo todos los valores en el paquete
    agregar_a_paquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregar_a_paquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregar_a_paquete(paquete,&contextoEjecucion->registrosCPU, sizeof(contextoEjecucion->registrosCPU)); // a chequear ese ampersand
    agregar_a_paquete(paquete,(void *)&contextoEjecucion->instruccionesLength, sizeof(contextoEjecucion->instruccionesLength));
    agregar_a_paquete(paquete,contextoEjecucion->instrucciones, contextoEjecucion->instruccionesLength);
    agregar_a_paquete(paquete,(void *)contextoEjecucion->estado, sizeof(estadoProceso));

    enviar_paquete(paquete,socketCliente);

	eliminar_paquete(paquete);
}




//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU


t_contexto* recibir_contexto(){
    //deserializar

    t_contexto* nuevoContexto = malloc(sizeof(t_contexto));
	int size;
	int desplazamiento = 0;
	void * buffer;


	buffer = recibir_buffer(&size);
	while(desplazamiento < size){ //segun entiendo el while hace que se quede esperando a recbibir absoulamente todos los datos
        memcpy(&(nuevoContexto->pid), buffer+desplazamiento, sizeof(nuevoContexto->pid));
        desplazamiento+=sizeof(nuevoContexto->pid);
        memcpy(&(nuevoContexto->programCounter), buffer+desplazamiento, sizeof(nuevoContexto->programCounter));
        desplazamiento+=sizeof(nuevoContexto->programCounter);
        memcpy(&(nuevoContexto->registrosCPU), buffer+desplazamiento, sizeof(nuevoContexto->registrosCPU));
        desplazamiento+=sizeof(nuevoContexto->registrosCPU);
    
        memcpy(&(nuevoContexto->instruccionesLength), buffer+desplazamiento, sizeof(nuevoContexto->instruccionesLength));
        desplazamiento += sizeof(nuevoContexto->instruccionesLength);
        nuevoContexto->instrucciones = malloc(nuevoContexto->instruccionesLength);
        memcpy(nuevoContexto->instrucciones , buffer+desplazamiento, nuevoContexto->instruccionesLength);
		
        memcpy(&(nuevoContexto->estado), buffer+desplazamiento, sizeof(estadoProceso));
        desplazamiento+=sizeof(estadoProceso);
		
	}


	free(buffer);
	return nuevoContexto;
}

