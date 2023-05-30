#include "kernel/include/configuraciones.h"

// Configuracion

double obtenerEstimacionInicial()
{
    return config_get_double_value(config, "ESTIMACION_INICIAL");
}

int obtenerGradoMultiprogramacion()
{
    return config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
}

char *obtenerAlgoritmoPlanificacion()
{
    return confGet("ALGORITMO_PLANIFICACION");
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