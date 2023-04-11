UNAME:=$(shell uname)

ifeq ($(UNAME),Linux)
FILE_EXT:=out
else
FILE_EXT:=exe
endif

SHARED := /shared/src

BIN := /bin/

ARCHIVOS_CONSOLA := /consola/src/consola.c 
ARCHIVOS_CPU := /cpu/src/cpu.c /cpu/src/conexionMemoria.c /cpu/src/servidorKernel.c
ARCHIVOS_FILESYS := /file-system/src/fileSystem.c 
ARCHIVOS_KERNEL := /kernel/src/kernel.c 
ARCHIVOS_MEMORIA := /memoria/src/memoria.c /memoria/src/utils.c 

UTILS := $(SHARED)/utilsServidor.c $(SHARED)/utilsCliente.c $(shared)/configuraciones.c

CC := gcc
CFLAGS := -I ./ -lcommons -Wall -Wextra -O

trap: 
	@echo "Si estas aca te confundiste de ondis, pone \'make MODULO\' con el modulo que queres compilar."

consola:
	@echo "Compilando consola... "
	@$(CC) $(ARCHIVOS_CONSOLA) $(UTILS) -o $(BIN)consola.$(FILE_EXT) $(CFLAGS) -lreadline

cpu:
	@echo "Compilando CPU... "
	@$(CC) $(ARCHIVOS_CPU) $(UTILS) -o $(BIN)cpu.$(FILE_EXT) $(CFLAGS)

filesys:
	@echo "Compilando File System... "
	@$(CC) $(ARCHIVOS_FILESYS) $(UTILS) -o $(BIN)filesys.$(FILE_EXT) $(CFLAGS)

kernel:
	@echo "Compilando Kernel... "
	@$(CC) $(ARCHIVOS_KERNEL) $(UTILS) -o $(BIN)kernel.$(FILE_EXT) $(CFLAGS)

memoria:
	@echo "Compilando Memoria... "
	@$(CC) $(ARCHIVOS_MEMORIA) $(UTILS) -o $(BIN)memoria.$(FILE_EXT) $(CFLAGS)