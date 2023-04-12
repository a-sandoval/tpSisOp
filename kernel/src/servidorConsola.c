/* CONSOLA - cliente | KERNEL - sevidor*/

// AVISO QUE ESTO ESTÁ PATEADÍSIMO ME FALTA TOQUETEARLO DE ARRIBA A ABAJO
// SI ALGUIEN LO VE NO SE ASUSTE!!!

#include "kernel/include/servidorConsola.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	int ejecutarServidor(int cliente_fd, t_log* logger)
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}