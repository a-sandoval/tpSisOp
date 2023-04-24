#ifndef GLOBAL_H_
#define GLOBAL_H_

    #include <commons/log.h>
    #include <commons/config.h>

    t_log* logger;
    t_config* config;
    t_list* clavesValidas;
    volatile sig_atomic_t pararPrograma;

#endif
