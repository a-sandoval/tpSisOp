#!/bin/bash

./consola.out ./tuki-pruebas/DEADLOCK_1 ./consola.config &
sleep 0.2; ./consola.out ./tuki-pruebas/DEADLOCK_2 ./consola.config &
sleep 0.2; ./consola.out ./tuki-pruebas/DEADLOCK_3 ./consola.config &
sleep 3; ./consola.out ./tuki-pruebas/DEADLOCK_4 ./consola.config &
