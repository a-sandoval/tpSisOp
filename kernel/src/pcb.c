#include "kernel/include/pcb.h"

char* pidsInvolucrados; 

//Básicos PCB

t_pcb *crearPCB(){

    t_pcb *nuevoPCB = malloc(sizeof(t_pcb));
    nuevoPCB->estado = NEW;
    nuevoPCB->pid = procesosCreados;
    nuevoPCB->programCounter = 0;
    nuevoPCB->instrucciones = list_create();
    nuevoPCB->estimadoProximaRafaga = obtenerEstimacionInicial();
    //nuevoPCB->tablaDeArchivos = list_create();
    //nuevoPCB->tablaDeSegmentos = list_create();
    nuevoPCB->registrosCPU = crearDiccionarioDeRegistros();

    procesosCreados++; // para el nuevo pid
    

    return nuevoPCB;
}

void destruirPCB(t_pcb *pcb){
    list_destroy_and_destroy_elements(pcb->instrucciones, (void *)destruirInstruccion);
    //list_destroy_and_destroy_elements(pcb->tablaDeArchivos, (void *)destruirInstruccion);
    //list_destroy_and_destroy_elements(pcb->tablaDeSegmentos, (void *)destruirInstruccion);
    dictionary_destroy_and_destroy_elements(pcb->registrosCPU, (void *)destruirRegistro);
    free(pcb);
}

t_dictionary *crearDiccionarioDeRegistros(){

    t_dictionary *registros = dictionary_create();

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
    dictionary_put(registros, "AX", AX);
    dictionary_put(registros, "BX", BX);
    dictionary_put(registros, "CX", CX);
    dictionary_put(registros, "DX", DX);
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

void destruirListaPCB(t_list *pcbs){
    list_destroy_and_destroy_elements(pcbs, (void *)destruirPCB);
}

void encolar(t_list *pcbs, t_pcb *pcb){
    list_add(pcbs, (void *)pcb);
}

t_pcb *desencolar(t_list *pcbs){
    return (t_pcb *)list_remove(pcbs, 0);
}

void agregarPID(void *value){
    t_pcb *pcb = (t_pcb *)value;
    char *pid = string_itoa(pcb->pid);
    string_append_with_format(&pidsInvolucrados, " %s ", pid);
}

void listarPIDS(t_list *pcbs){
    list_iterate(pcbs, agregarPID);
}


// frees

void destruirInstruccion(t_instruccion *instruccion){
    free(instruccion);
}

void destruirRegistro(char *registro){
    free(registro);
}




