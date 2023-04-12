UNAME:=$(shell uname)

ifeq ($(UNAME),Linux)
FILE_EXT:=.out
else
FILE_EXT:=.exe
endif

SHARED := shared/src
CONFIG         := $(SHARED)/configuraciones.c
UTILS_CLIENTE  := $(SHARED)/utilsCliente.c
UTILS_SERVIDOR := $(SHARED)/utilsServidor.c


ARCHIVOS_CONSOLA := consola/src/consola.c                                              $(CONFIG) $(UTILS_CLIENTE)
CPU_SRC := cpu/src/
ARCHIVOS_CPU := $(CPU_SRC)cpu.c $(CPU_SRC)conexionMemoria.c $(CPU_SRC)servidorKernel.c $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR)
ARCHIVOS_FILESYS := file-system/src/fileSystem.c                                       $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR)
ARCHIVOS_KERNEL := kernel/src/kernel.c                                                 $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR)
ARCHIVOS_MEMORIA := memoria/src/memoria.c memoria/src/utils.c                          $(CONFIG)                  $(UTILS_SERVIDOR)


CC := gcc
CFLAGS := -g -I ./ -lcommons -Wall -Wextra

trap: 
	@echo "Si estas aca te confundiste de ondis, pone \'make comp_MODULO\' con el modulo que queres compilar."

comp_consola:
	@echo "Compilando consola... "
	@$(CC) $(ARCHIVOS_CONSOLA) -o consola/consola$(FILE_EXT)     $(CFLAGS) -lreadline

comp_cpu:
	@echo "Compilando CPU... "
	@$(CC) $(ARCHIVOS_CPU)     -o cpu/cpu$(FILE_EXT)             $(CFLAGS)

comp_filesys:
	@echo "Compilando File System... "
	@$(CC) $(ARCHIVOS_FILESYS) -o file-system/filesys$(FILE_EXT) $(CFLAGS)

comp_kernel:
	@echo "Compilando Kernel... "
	@$(CC) $(ARCHIVOS_KERNEL)  -o kernel/kernel$(FILE_EXT)       $(CFLAGS)

comp_memoria:
	@echo "Compilando Memoria... "
	@$(CC) $(ARCHIVOS_MEMORIA) -o memoria/memoria$(FILE_EXT)     $(CFLAGS)