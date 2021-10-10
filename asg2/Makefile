make:
	mpicc -o asg2_s asg2_s.c
	mpicc -lm -o asg2_p asg2_p.c

run_s:
	./asg2_s

run_p:
	mpirun -np 32 --mca mpi_cuda_support 0 asg2_p

clean:
	rm asg2_p
	rm asg2_s
	rm out.ppm