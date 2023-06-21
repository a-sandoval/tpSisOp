#include "memoria/include/peticiones.h"

//serializar tabla de Segmentos

void enviarTablaSegmentos(t_proceso* procesoEnMemoria){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = TABLADESEGMENTOS;

    uint32_t tablaDeSegmentosSize = list_size(procesoEnMemoria->tablaDeSegmentosAsociada);

	agregarAPaquete(paquete,(void*)&procesoEnMemoria->pid,sizeof(uint32_t)); 
   
    uint32_t i;
    for(i=0;i<tablaDeSegmentosSize;i++){
        agregarSegmentoAPaquete(paquete,list_get(procesoEnMemoria->tablaDeSegmentosAsociada, i));
    }

    enviarPaquete(paquete, sockets[0]);

	eliminarPaquete(paquete);
}

void agregarSegmentoAPaquete(t_paquete* paquete, t_segmento* segmento){
	agregarAPaquete(paquete, (void*)&segmento->id, sizeof(uint32_t));
	agregarAPaquete(paquete, (void*)&segmento->direccionBase, sizeof(uint32_t));
	agregarAPaquete(paquete, (void*)&segmento->tamanio, sizeof(uint32_t));
}

uint32_t recibirPID(int socketCliente) {

	int size, desplazamiento=0; 
	uint32_t pid; 

	void* buffer = recibirBuffer(socketCliente, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(uint32_t));

	return pid; 

}

t_peticion* recibirPeticionCreacionDeSegmento(int socketCliente) {

	int size, desplazamiento = 0; 

	t_peticion* peticion = malloc(sizeof(t_peticion)); 

	t_segmento* segmentoPedido = malloc(sizeof(t_segmento)); 

	peticion->segmento = segmentoPedido; 

	void* buffer = recibirBuffer(socketCliente, &size); 

	desplazamiento+=sizeof(uint32_t); 

	memcpy(&peticion->pid, buffer + desplazamiento, sizeof(uint32_t));

	desplazamiento+=(2*sizeof(uint32_t)); 

	memcpy(&peticion->segmento->id,buffer+desplazamiento,sizeof(uint32_t)); 

	desplazamiento +=(3*sizeof(uint32_t)); 

	memcpy(&peticion->segmento->direccionBase, buffer+desplazamiento,sizeof(uint32_t)); 

	return peticion; 

}

void recibirYProcesarPeticionEliminacionSegmento(int socketCliente) {

	int size, desplazamiento = 0; 

	uint32_t pid; 

	uint32_t segmentId; 

	void* buffer = recibirBuffer(socketCliente, &size); 

	desplazamiento+=sizeof(uint32_t); 

	memcpy(&pid, buffer + desplazamiento, sizeof(uint32_t));

	desplazamiento+=(2*sizeof(uint32_t)); 

	memcpy(&segmentId,buffer+desplazamiento,sizeof(uint32_t)); 

	deleteSegment(pid, segmentId); 
}