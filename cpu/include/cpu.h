#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>

#include "shared/include/configuraciones.h"
#include "shared/include/utilsCliente.h"
#include "shared/include/utilsServidor.h"
#include "cpu/include/servidorKernel.h"

int socketCliente;
t_log* logger;
t_config* config;

int conexionMemoria();

//FUNCION PARA RECIBIR NUEVO CONTEXTO POR PARTE DEL KERNEL
t_contexto* recibir_contexto(){
    t_contexto* nuevoContexto = malloc(sizeof(t_contexto));
	int size;
	int desplazamiento = 0;
	void * buffer;

	buffer = recibir_buffer(&size);
	while(desplazamiento < size){ 
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



#endif 