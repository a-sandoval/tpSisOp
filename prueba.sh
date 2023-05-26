#!/bin/sh

width=$(tput cols)
height=$(tput lines)

first_screen=$(($height/2))

tmux new-session -s tpSisOp -d -x $width -y $height -c cpu "make; ./cpu.out; exec bash -i" 
tmux split-window -t tpSisOp:0.0 -l "$first_screen" -c kernel "make; ./kernel.out; exec bash -i"
tmux split-window -t tpSisOp:0.1 -l 5 -c consola "make; for i in {1..5}; do make run; done; exec bash -i"
tmux attach
