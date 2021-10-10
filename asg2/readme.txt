There is an included make file in this submission that should allow everything to compile and run correctly

make run_s will run the serial implmentation of asg2
make run_p will run the parallel implmentation of asg2

The make file is currently set to run asg2_p with 32 procs at 10,000 iterations

These two variables can easily be changed. The proc amount is in the make file and the iteration amount is at the beginning of the main function in asg2_p.c

If for some reason the make file does not work to compile asg2_s.c use...
mpicc -o asg2_s asg2_s.c

To run asg2_s.c...
./asg2_s

And for asg2_p.c use...	
mpicc -lm -o asg2_p asg2_p.c


To run asg2_p.c...
mpirun -np 32 --mca mpi_cuda_support 0 asg2_p

Team:
I did this project individually.

Assumptions:
1)That parallel code asg2_p will not be run with less than 2 mpi processes.
2)Run on palmetto with 32 cpus and mpiprocs or change variables as mentioned above to work with lower cpu counts.
3)That no Dynamic input is required for iteration count.
4)That .ppm is an acceptable file extention. Can be opened with GIMP or 
converted on a linux system that has access to the convert command.
Can also use https://convertio.co/ppm-png/ to convert.
5)That dimensions are square and dont need to be read from input.