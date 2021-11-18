estPi or Estimate Pi uses RNG and an equation (x*x + y*y <= 1) to estimate pi.

This works by counting the number of RNG x and y values that satisfy this equation then dividing the count by the sample number to produce a pi estimate.

estPi is the fastest implementation using MPI_Reduce.
estPi_g is a bit slower and uses MPI_Gather.
estPi_s is a sequential version of the code.

make then make run will compile and run these files in the order listed.

estPi expects a single number in the command line representing the number of samples to be taken. The higher the sample number the more accurate the estimation of pi.

mpirun -np 32 --mca mpi_cuda_support 0 estPi 1000000000  
    Count = 785492832, Sample = 1000000000, Estimate of pi = 3.14197, Time = 1.855s

mpirun -np 32 --mca mpi_cuda_support 0 estPi_g 1000000000  
    Count = 764944860, Sample = 1000000000, Estimate of pi = 3.05978, Time = 2.200s

mpirun -np 32 --mca mpi_cuda_support 0 estPi_s 1000000000  
    Count = 785400903, Sample = 1000000000, Estimate of pi = 3.14160, Time = 21.404s

./estPi 1000000000  
    Count = 785402979, Sample = 1000000000, Estimate of pi = 3.14161, Time = 33.944s