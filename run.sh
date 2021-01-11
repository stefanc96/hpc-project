#!/bin/bash

mpicc root.c -o root
mpicc worker.c -o worker
#First argument is rows, second is cols, last is number of elements for a worker
mpirun -np 1 --oversubscribe  root 6 6 3
