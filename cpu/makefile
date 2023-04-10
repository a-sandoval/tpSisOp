cpu: cpu.o conexionMemoria.o utils.o utilsServidor.o servidorKernel.o
	gcc cpu.o conexionMemoria.o utils.o utilsServidor.o servidorKernel.o -o cpu -lfl -lm -lcommons -lreadline

utils.o: ../shared/src/utils.c ../shared/include/utils.h
	gcc -c ../shared/src/utils.c

utilsServidor.o: ../shared/src/utilsServidor.c ../shared/include/utilsServidor.h
	gcc -c ../shared/src/utilsServidor.c
	
servidorKernel.o: src/servidorKernel.c include/servidorKernel.h
	gcc -c src/servidorKernel.c

cpu.o: src/cpu.c include/cpu.h
	gcc -c src/cpu.c

conexionMemoria.o: src/conexionMemoria.c include/conexionMemoria.h
	gcc -c src/conexionMemoria.c

clean:
	rm cpu.o conexionMemoria.o utils.o utilsServidor.o servidorKernel.o cpu