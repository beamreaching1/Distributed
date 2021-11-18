#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	double t;
	t = MPI_Wtime();
	int i, count;
	double x, y;
	int samples;
	double pi;
	int rank, numtasks;
	srand(time(NULL));

	samples = atoi(argv[1]);

	count = 0;
	for (i = 0; i < samples; i++){
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;
		if(x*x + y*y <= 1){
			count++;
		}
	}

	pi = 4.0 * (double)count/(double)samples;
	t = MPI_Wtime() - t;
	printf("Count = %d, Sample = %d, Estimate of pi = %7.5f, Time = %1.3fs\n", count, samples, pi, t);
	return 0;
}