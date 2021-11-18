make:
	mpicc -o heat_s heat_s.c
	mpicc -lm -o heat_p heat_p.c

run_s:
	./heat_s

run_p:
	mpirun -np 32 --mca mpi_cuda_support 0 heat_p

clean:
	rm heat_p
	rm heat_s
	rm out.ppm