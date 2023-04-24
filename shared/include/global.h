#ifndef GLOBAL_H_
#define GLOBAL_H_

    #include <commons/log.h>
    #include <commons/config.h>

    t_log* logger;
    t_config* config;

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
