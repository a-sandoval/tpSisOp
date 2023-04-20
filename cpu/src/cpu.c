#include "cpu/include/cpu.h"

int main(void){

	Lista *clavesValidas = malloc(sizeof(Lista));
	clavesValidas->cabeza = NULL;

	/*Inicializando Loggers*/
	logger = iniciarLogger("CPUcliente.log", "CPU-Memoria");

	/*Inicializando los config*/
	config = iniciarConfiguracion("cpu.config", logger);
	insertar(clavesValidas, confGet(config, "CLAVE_KERNEL_CPU"));
	imprimirLista(clavesValidas);

	/*Conexion a memoria*/
	int conexion_memoria = conexionMemoria(logger);
	if(conexion_memoria == -1){
		log_error(logger, "No se pudo crear la conexion con la memoria");
		terminarPrograma(conexion_memoria, clavesValidas);
		return EXIT_FAILURE; //abort o exit?
	}


	/*Preparacion de la cpu para servir al kernel*/
	cambiarNombre(logger, "CPU-Kernel");
	int conexion_servir_kernel = alistarServidor(logger, confGet(config,"PUERTO_ESCUCHA"));

	ejecutarServidor(conexion_servir_kernel, logger, clavesValidas);

	terminarPrograma(conexion_memoria, clavesValidas);

    return EXIT_SUCCESS;
}

void iterator(char *value){
	log_info(logger, "Valor recibido: %s\n",value);
}

void terminarPrograma(int conexion, Lista*lista)
{
	log_destroy(logger);
	config_destroy(config);
	borrarLista(lista);
	liberar_conexion(conexion);
}

int conexionMemoria(){

	/* Variables */

	int conexion;
	char* valor = confGet(config,"CLAVE_CPU_MEMORIA");
	char* ip = confGet(config,"IP_MEMORIA");
	char* puerto = confGet(config,"PUERTO_MEMORIA");

	/*Configuraciones*/
	log_info(logger, "VALOR: %s, IP: %s, PUERTO:%s", valor, ip, puerto);

	conexion = realizarConexion(ip,puerto,valor);
	log_info(logger, "VALOR DE LA CONEXION: %d", conexion);

	/*Liberar memoria*/
	/*free(valor);
	free(ip);
	free(puerto);*/

	return conexion;

}
