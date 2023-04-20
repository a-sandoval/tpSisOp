# tp-2023-1c-toTheEnd

## Módulos

- Consola: La forma de comunicarse entre el usuario y la computadora.
- Kernel: El interprete entre el hardware y el software.
- CPU: El procesador principal que traduce las realizqa del Kernel.
- Memoria: El almacenamiento predilecto para uso de las aplicaciones.
- File System: El almacenamiento mas amplio (y mas lento) para el guardado de información.

## Interconexión

El sistema va a estar conectado de la siguiente manera: (Perdon pruebo a ver si me anda bien GitHUb en VsCode)

La(s) consola(s) se va(n) a conectar al kernel. (Estas van a ir ocupando los puertos a medida que se vayan conectando hasta tener una cantidad máxima de consolas abiertas, y ademáś se va a poder diferenciar si una consola entra en modo Kernel o modo Usuario segúń la clave que se le de al kernel).

El Kernel va a conectarse, según los mensajes de las consolas, y segúń las respuestas que recibe, a la memoria, al CPU, y al File System.

El CPU va a recibir la conexión solicitada del Kernel y cuando véa necesario va a conectarse a la memoria.

El File System actua de igual manera que el CPU.

Y la memoria va a actuar como servidor principal recibiendo paquetes del CPU, el File System, y el Kernel.

## Puertos

Se deben considerar 5 + la cantidad de consolas máximas de puertos distintos para que el sistema funcione sin que se mezclen los puertos:

Los tres puertos que recibe la memoria (por el CPU, FS, y Kernel): 

Los dos puertos individuales del CPU y el FS: 

Y los puertos del Kernel para recibir a todas las consolas: 

(Para los puertos, cualquiera entre 1024 y 65535 sirve, recomendaria usar uno mayor a 10000 por su rareza)

## Claves

Se van a utilizar 7 claves distintas para comunicar a los distintos módulos durante el handshake:

1. De la consola en modo usuario al kernel: "usuario"
2. De la consola en modo kernel al kernel: "admin"
3. Del kernel al CPU: "kernel-cpu"
4. Del kernel al FS: "kernel-fs"
5. Del kernel a la memoria: "kernel-mem"
6. Del CPU a la memoria: "cpu-mem"
7. Del FS a la memoria: "fs-mem"

(De otra manera se podria utiilzar un enum simple)

## Tareas para este checkpoint

### Módulo Consola

- [X] Abre archivo de configuración y logger.
- [X] Se conecta a Kernel y envia instrucciones.
- [X] Globalizar configuracion, loggers y sockets. (Fede)

### Módulo Kernel

- [X] Abre archivo de configuración y logger.
- [X] Se conecta a CPU, Memoria y File System.
- [ ] Esperar constantemente conexiones de consolas (Tal vez llegar a mas de 1).
- [ ] Recibir instrucciones y armar el PCB.
- [ ] Planificacion de procesos con FIFO
- [ ] Planear como agregar el HNNR (HRRN?).
- [ ] Configurar para que se mantenga recibiendo conexiones indefinidamente hasta que se termine el programa. (Fede)
- [X] Globalizar configuracion, loggers y sockets. (Fede)

### Módulo CPU

- [X] Abre archivo de configuración y logger.
- [X] Se conecta a memoria, espera conexiones de Kernel.
- [ ] Ejecutar instrucciones:
    - [ ] SET X Y (Setear al registro X el valor Y).
    - [ ] YIELD (Desalojar voluntariamente el proceso de la CPU).
    - [ ] EXIT (Syscall de finalización de proceso).
- [ ] Configurar para que se mantenga recibiendo conexiones indefinidamente hasta que se termine el programa. (Fede)
- [X] Globalizar configuracion, loggers y sockets. (Fede)

### Módulo File System

- [X] Abre archivo de configuración y logger.
- [X] Se conecta a memoria, espera conexiones de Kernel.
- [ ] Configurar para que se mantenga recibiendo conexiones indefinidamente hasta que se termine el programa. (Fede)
- [X] Globalizar configuracion, loggers y sockets. (Fede)

### Módulo Memoria

- [X] Abre archivo de configuración y logger.
- [X] Recibir conexiones de CPU, Kernel y File System.
- [ ] Configurar para que se mantenga recibiendo conexiones indefinidamente hasta que se termine el programa. (Fede)
- [X] Globalizar configuracion, loggers y sockets. (Fede)

### Módulo Shared

- [ ] Documentar funciones.
- [X] Añadir global.h para todas las variables globales que necesitemos generalmente (logger, config, etc.).
- [ ] Traspasar toda dependencia de nuestras listas al commons/collections/list.h.

### Módulo pruebas

- [ ] Preparar makefile general para correr pruebas entre módulos.
- [ ] Hacer pruebas entre dos computadoras distintas. ("ip addr show | grep 192" te deberia mostrar que ip tenes en la red local)