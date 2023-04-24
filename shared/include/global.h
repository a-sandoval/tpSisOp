#ifndef GLOBAL_H_
#define GLOBAL_H_

    #include <commons/log.h>
    #include <commons/config.h>

    int socketClienteFD;
    t_log* logger;
    t_config* config;
    t_list* clavesValidas;
    volatile sig_atomic_t pararPrograma;

    typedef enum t_comando {

    SET,
    MOV_IN,
    MOV_OUT, 
    IO,
    F_OPEN,
    F_CLOSE, 
    F_SEEK,
    F_READ,
    F_WRITE, 
    F_TRUNCATE,
    WAIT,
    SIGNAL,
    CREATE_SEGMENT,
    DELETE_SEGMENT,
    YIELD,
    EXIT, 

} t_comando; 

char *listaComandosGlobal[] = {

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
    [EXIT] = "EXIT", 

};

typedef struct t_instruccion {

    t_comando comando; 
    char* parametros[3]; 

}t_instruccion; 

typedef enum t_comando {

    SET,
    MOV_IN,
    MOV_OUT, 
    IO,
    F_OPEN,
    F_CLOSE, 
    F_SEEK,
    F_READ,
    F_WRITE, 
    F_TRUNCATE,
    WAIT,
    SIGNAL,
    CREATE_SEGMENT,
    DELETE_SEGMENT,
    YIELD,
    EXIT, 

} t_comando; 

typedef struct t_instruccion {

    t_comando comando; 
    char* parametros[3]; 

}t_instruccion; 

#endif
