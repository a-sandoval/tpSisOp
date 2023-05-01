/* KERNEL- cliente | CPU - sevidor*/
#include "kernel/include/conexionCPU.h"

typedef struct  {

//ver tipos de datos para hacer el sizeof mas legible
    uint32_t pid; 
    int socketPCB; 
    t_list* instrucciones; 
    uint32_t instruccionesLength;
    int programCounter;   
    t_reg registrosCPU;   // Puntero o no? 

    
    //TO DO: TABLA DE SEGMENTOS Y TABLA DE ARCHIVOS 

} t_contexto; 

t_buffer bufferContexto;
t_contexto* contextoEjecucion;

int conexionCPU() {

    logger = cambiarNombre("Kernel-CPU");
    int conexionACPU = conexion("CPU");
    if(!(conexionACPU + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else{
        //enviar_contexto();
        close(conexionACPU);

    }
    
 
}


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

    paquete->buffer->size = sizeof(contextoEjecucion->pid)+sizeof(contextoEjecucion->socketPCB)+
                            sizeof(contextoEjecucion->programCounter)+sizeof(contextoEjecucion->registrosCPU)+
                            sizeof(contextoEjecucion->instruccionesLength)+strlen(contextoEjecucion->instrucciones); //noc si funciona esto de las instrucciones
    paquete->buffer->stream = malloc(paquete->buffer->size);
    */
    // hasta aca indique el tamaño del buffer y pedi el lugar necesario para poder cargar el stream con los valores a enviar
    // cargo todos los valores en el paquete
    agregar_a_paquete(paquete,contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregar_a_paquete(paquete,contextoEjecucion->socketPCB, sizeof(contextoEjecucion->socketPCB));
    agregar_a_paquete(paquete,contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregar_a_paquete(paquete,&contextoEjecucion->registrosCPU, sizeof(contextoEjecucion->registrosCPU)); // a chequear ese ampersand
    agregar_a_paquete(paquete,contextoEjecucion->instruccionesLength, sizeof(contextoEjecucion->instruccionesLength));
    agregar_a_paquete(paquete,contextoEjecucion->instrucciones, contextoEjecucion->instruccionesLength);

    enviar_paquete(paquete,socketCliente);

	eliminar_paquete(paquete);
}


void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio){
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente){
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_contextoEjecucion(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete){
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}