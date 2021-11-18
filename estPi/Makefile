CC=gcc
CFLAGS=-I.

make:
	mpicc -o estPi asg1.c
	mpicc -o estPi_s estPi_s.c
	mpicc -o estPi_g estPi_g.c

clean:
	rm estPi
	rm estPi_s
	rm estPi_g

run:
	mpirun -np 32 --mca mpi_cuda_support 0 estPi 1000000000
	mpirun -np 32 --mca mpi_cuda_support 0 estPi_g 1000000000
	mpirun -np 32 --mca mpi_cuda_support 0 estPi_s 1000000000
