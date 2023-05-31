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


ARCHIVOS_CONSOLA := consola/src/consola.c \
                    $(CONFIG) $(UTILS_CLIENTE) 

CPU_SRC := cpu/src/
ARCHIVOS_CPU     := $(CPU_SRC)cpu.c	$(CPU_SRC)servidorKernel.c $(CPU_SRC)cicloDeInstruccion.c $(CPU_SRC)conexionMemoria.c $(CPU_SRC)contextoEjecucion.c  \
				    $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR) 

FILESYS_SRC := fileSystem/src/
ARCHIVOS_FILESYS := $(FILESYS_SRC)fileSystem.c $(FILESYS_SRC)servidorKernel.c \
				    $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR) 

ARCHIVOS_KERNEL  := kernel/src/kernel.c kernel/src/conexionMemoria.c  kernel/src/conexionCPU.c  kernel/src/conexionFileSystem.c  kernel/src/servidorConsola.c kernel/src/planificacion.c kernel/src/contextoEjecucion.c kernel/src/algoritmosCortoPlazo.c kernel/src/syscalls.c kernel/src/configuraciones.c kernel/src/pcb.c kernel/src/manejoRecursos.c\
                    $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR) 

ARCHIVOS_MEMORIA := memoria/src/memoria.c \
                    $(CONFIG)                  $(UTILS_SERVIDOR) 

ARCHIVOS_TEST := test/src/test.c \
                    $(CONFIG) $(UTILS_CLIENTE) $(UTILS_SERVIDOR) 

CC := gcc
CFLAGS := -g -I ./ -lcommons -Wall -Wextra

trap:
	@echo "Si estas aca te confundiste de ondis, pone \'make Modulo\' con el modulo que queres compilar o pone make en tu carpeta."

all: Consola Cpu Filesys Kernel Memoria
	@echo "Estas demente "

Consola:
	@echo "Compilando consola... "
	@$(CC) $(ARCHIVOS_CONSOLA) -o consola/consola$(FILE_EXT)     $(CFLAGS) -lreadline

Cpu:
	@echo "Compilando CPU... "
	@$(CC) $(ARCHIVOS_CPU)     -o cpu/cpu$(FILE_EXT)             $(CFLAGS)

Filesys:
	@echo "Compilando File System... "
	@$(CC) $(ARCHIVOS_FILESYS) -o fileSystem/filesys$(FILE_EXT) $(CFLAGS)

Kernel:
	@echo "Compilando Kernel... "
	@$(CC) $(ARCHIVOS_KERNEL)  -o kernel/kernel$(FILE_EXT)       $(CFLAGS)

Memoria:
	@echo "Compilando Memoria... "
	@$(CC) $(ARCHIVOS_MEMORIA) -o memoria/memoria$(FILE_EXT)     $(CFLAGS) -lpthread

Test:
	@echo "Compilando Servidor Prueba... "
	@$(CC) $(ARCHIVOS_TEST) -o test/test$(FILE_EXT)     $(CFLAGS)

clean:
	rm cpu/cpu.o