Heat is a simple heat distribution simulation. It is designed to distributed sections of the simulation to different cpu cores.
Heat also handles the boundary cases between each distributed section.
The output is a color map of the heat distribution after x amount of iterations.

<img
src=./output.png”
raw=true
alt=“Heat Map Output”
style=“margin-right: 10px;”
/>

There is an included make file that should allow everything to compile and run correctly.

Run make before running any other commands.

make run_s will run the serial implementation of heat
make run_p will run the parallel implementation of heat

The make file is currently set to run heat_p with 32 procs at 10,000 iterations

These two variables can easily be changed. The proc amount is in the make file and the iteration amount is at the beginning of the main function in heat_p.c

If for some reason the make file does not work to compile heat_s.c use...
mpicc -o heat_s heat_s.c

To run heat_s.c...
./heat_s

And for heat_p.c use...	
mpicc -lm -o heat_p heat_p.c


To run heat_p.c...
mpirun -np 32 --mca mpi_cuda_support 0 heat_p

.ppm is current output format. You can open it with GIMP or 
converted on a linux system that has access to the convert command.
Can also use https://convertio.co/ppm-png/ to convert.

Assumptions:
1)That parallel code heat_p will not be run with less than 2 mpi processes.
2)Run with 32 mpiprocs or change variables as mentioned above to work with lower cpu counts.