#!/bin/bash

./consola.out ./tuki-pruebas/BASE_1 ./consola.config &
sleep 1; ./consola.out ./tuki-pruebas/BASE_2 ./consola.config &
sleep 1; ./consola.out ./tuki-pruebas/BASE_2 ./consola.config 
