#include "kernel/include/configuraciones.h"

// Configuracion

int obtenerGradoMultiprogramacion()
{
    return config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
}

double obtenerEstimacionInicial()
{
    return config_get_double_value(config, "ESTIMACION_INICIAL");
}


char *obtenerAlgoritmoPlanificacion()
{
    return config_get_string_value(config,"ALGORITMO_PLANIFICACION");
}

double obtenerAlfaEstimacion()
{
    return config_get_double_value(config, "HRRN_ALFA");
}

char **obtenerRecursos() {

    return config_get_array_value(config, "RECURSOS");
}

char **obtenerInstanciasRecursos(){
    return config_get_array_value(config, "INSTANCIAS_RECURSOS");
}