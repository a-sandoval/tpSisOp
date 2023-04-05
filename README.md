# tp-2023-1c-toTheEnd

## Módulos

- Consola: La forma de comunicarse entre el usuario y la computadora.
- Kernel: El interprete entre el hardware y el software.
- CPU: El procesador principal que traduce las realizqa del Kernel.
- Memoria: El almacenamiento predilecto para uso de las aplicaciones.
- File System: El almacenamiento mas amplio (y mas lento) para el guardado de información.

## Interconexión

El sistema va a estar conectado de la siguiente manera:

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

- Cada módulo debe tener su archivo de configuración respectivo (ver consigna para formato).
    - [ ] Consola
    - [ ] CPU
    - [ ] File System
    - [ ] Kernel
    - [ ] Memoria
- Cada módulo debe tener su conjunto de funciones de cliente y/ó servidor para conectarse con otros módulos.
    - [ ] Consola
    - [ ] CPU
    - [ ] File System
    - [ ] Kernel
    - [ ] Memoria

De ser posible:

- Cada módulo debe abrir sus respectivos puertos y esperar conexiones entrantes (sin necesidad de preparar el handshake).
    - [ ] CPU
    - [ ] File System
    - [ ] Kernel
    - [ ] Memoria

- Cada módulo debe conectarse al servidor requerido (sin necesidad de preparar el handshake).
    - [ ] Consola
    - [ ] CPU
    - [ ] File System
    - [ ] Kernel

