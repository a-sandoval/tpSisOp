UNAME:=$(shell uname)

ifeq ($(UNAME),Linux)
FILE_EXT:=out
else
FILE_EXT:=exe
endif

SRC := src
SHARED := ../shared/src

FILES := $(SRC)/kernel.c $(SHARED)/utilsServidor.c $(SHARED)/utilsCliente.c
OUTPUT_FILE := kernel

CC := gcc
CFLAGS := -I ../ -lcommons -lreadline -Wall -Wextra -O

compile: 
	@echo "Compilando... "
	$(CC) $(FILES) -o $(OUTPUT_FILE).$(FILE_EXT) $(CFLAGS)