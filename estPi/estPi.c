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
	srand(time(NULL));

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
	MPI_Status Stat;
	int tag = 1;
	//if (rank == 0){
		/*for(int i = 0; i < size-1; i++){
			//printf("Recieved.\n");
			int temp = 0;
			MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
			count += temp;
		}*/
	//}
	//else{
		//int dest = 0;
		//MPI_Send(&count, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	//}
	
	int sendbuf = count;
	
	int recievebuf = 0;

	MPI_Reduce(&sendbuf, &recievebuf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if(rank == 0){
		pi = 4.0 * (double)recievebuf/(double)samples;
		t = MPI_Wtime() - t;
		printf("Count = %d, Sample = %d, Estimate of pi = %7.5f, Time = %1.3fs\n", recievebuf, samples, pi, t);
	}
	
	
	MPI_Finalize();
	return 0;
}
