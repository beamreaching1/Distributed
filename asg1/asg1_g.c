#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
	
	double t;
	t = MPI_Wtime();
	int i, count;
	double x, y;
	int samples;
	int size;
	double pi;
	int rank, numtasks;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	samples = atoi(argv[1]);
	srand(time(0)); 

	int task = samples / size;

	

	if (rank == 0) {
		int remainder = samples % size;
		task += remainder;
	}

	count = 0;
	for (i = 0; i < task; i++){
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;
		if(x*x + y*y <= 1){
			count++;
		}
	}

	int sumarr[size];
	//printf("Rank: %d Count: %d\n", rank, count);
	MPI_Gather(&count, 1, MPI_INT, &sumarr, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int count2;
	if(rank == 0){
		for (int i = 0; i < size-1; i++){
			count2 += sumarr[i];
		}
		pi = 4.0 * (double)count2/(double)samples;
		t = MPI_Wtime() - t;
		printf("Count = %d, Sample = %d, Estimate of pi = %7.5f, Time = %1.3fs\n", count2, samples, pi, t);
	}
	
	
	MPI_Finalize();
	return 0;
}