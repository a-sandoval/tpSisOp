cpu: cpu.o utilsCliente.o utilsServidor.o configuraciones.o
	gcc cpu.o utilsCliente.o utilsServidor.o configuraciones.o -o cpu -lfl -lm -lcommons -lreadline

configuraciones.o: ../shared/src/configuraciones.c ../shared/include/configuraciones.h
	gcc -c ../shared/src/configuraciones.c

utilsCliente.o: ../shared/src/utilsCliente.c ../shared/include/utilsCliente.h
	gcc -c ../shared/src/utilsCliente.c

utilsServidor.o: ../shared/src/utilsServidor.c ../shared/include/utilsServidor.h
	gcc -c ../shared/src/utilsServidor.c

cpu.o: src/cpu.c
	gcc -c src/cpu.c

clean:
	rm cpu.o utilsCliente.o utilsServidor.o configuraciones.o cpu