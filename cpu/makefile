cpu: cpu.o conexionMemoria.o utilsCliente.o utilsServidor.o servidorKernel.o
	gcc cpu.o conexionMemoria.o utilsCliente.o utilsServidor.o servidorKernel.o -o cpu -lfl -lm -lcommons -lreadline

utilsCliente.o: src/utilsCliente.c include/utilsCliente.h
	gcc -c src/utilsCliente.c

utilsServidor.o: src/utilsServidor.c include/utilsServidor.h
	gcc -c src/utilsServidor.c
	
servidorKernel.o: src/servidorKernel.c include/servidorKernel.h
	gcc -c src/servidorKernel.c

cpu.o: src/cpu.c include/cpu.h
	gcc -c src/cpu.c

conexionMemoria.o: src/conexionMemoria.c include/conexionMemoria.h
	gcc -c src/conexionMemoria.c

clean:
	rm cpu.o conexionMemoria.o utilsCliente.o utilsServidor.o servidorKernel.o cpu