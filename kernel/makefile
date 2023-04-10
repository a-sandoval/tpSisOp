#Por ser cliente
install:
	gcc -lcommons -lreadline src/utils.c src/kernel.c -o kernel.out

#Por ser servidor
UNAME:=$(shell uname)

ifeq ($(UNAME),Linux)
FILE_EXT:=out
else
FILE_EXT:=exe
endif

SRC := src

FILES := ./$(SRC)/main.c ./$(SRC)/utils.c
OUTPUT_FILE := kernel

CC := gcc
CFLAGS := -I $(SRC) -lcommons -lreadline -Wall -Wextra -O

compile: 
	@echo "Compilando... "
	@$(CC) $(FILES) -o $(OUTPUT_FILE).$(FILE_EXT) $(CFLAGS)