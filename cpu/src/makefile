cpu: cpu.o conexionMemoria.o utilsCliente.o utilsServidor.o servidorKernel.o
	gcc cpu.o conexionMemoria.o utilsCliente.o utilsServidor.o servidorKernel.o -o cpu -lfl -lm -lcommons -lreadline

utilsCliente.o: utilsCliente.c utilsCliente.h
	gcc -c utilsCliente.c

utilsServidor.o: utilsServidor.c utilsServidor.h
	gcc -c utilsServidor.c
	
servidorKernel.o: servidorKernel.c servidorKernel.h
	gcc -c servidorKernel.c

cpu.o: cpu.c cpu.h
	gcc -c cpu.c

conexionMemoria.o: conexionMemoria.c conexionMemoria.h
	gcc -c conexionMemoria.c

clean:
	rm cpu.o conexionMemoria.o utilsCliente.o utilsServidor.o servidorKernel.o cpu