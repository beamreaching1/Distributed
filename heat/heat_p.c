#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

const int DIM = 1000;

void calcNew(float new[][DIM], float old[][DIM], const int DIM);

void printOut(float out[][DIM], const int DIM, int rank, MPI_Datatype row, int size);

void update(float map[][DIM], int rank, int DIM_Y, MPI_Datatype row, int size);

int main(int argc, char *argv[]) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Custom MPI datatype
	MPI_Datatype row;
	MPI_Type_contiguous(DIM, MPI_FLOAT, &row);
	MPI_Type_commit(&row);

	const int FIREWIDTH = ((int)(DIM * 0.4)) - (((int)(DIM * 0.4)) % 2);
	const int ITERNUM = 100000;

	/*
	  Last process takes excess rows at bottom of room
	  Depending on rank leaves space at top and bottom for neighbor
	  arrays to send boundry rows
	*/ 
	int DIM_Y = 0;

	if(rank == 0 || rank == size-1){
		if(rank == size-1){
			DIM_Y = ceil((double)DIM/(double)size) + (DIM % size) + 1;
		} else {
			DIM_Y = floor((double)DIM/(double)size) + 1;
		}
	} else {
		DIM_Y = floor((double)DIM/(double)size) + 2;
	}
	
	float map1[DIM_Y][DIM];
	float map2[DIM_Y][DIM];

	//Ambient Temp init
	for (int i = 0; i < DIM_Y; i++) {
		for (int j = 0; j < DIM; j++) {
			map1[i][j] = 20.0;
			map2[i][j] = 20.0;
		}
	}

	//Fireplace init
	if (rank == 0) {
		for(int i = 0; i < FIREWIDTH; i++){
			map1[0][((DIM/2)-FIREWIDTH/2) + i] = 300.0;
			map2[0][((DIM/2)-FIREWIDTH/2) + i] = 300.0;
		}
	}
	
	//Loop for simulation
	for (int i = 0; i < ITERNUM; ++i) {
		if(i % 2 == 0){
			calcNew(map1, map2, DIM_Y);
			update(map1, rank, DIM_Y, row, size);
		} else {
			calcNew(map2, map1, DIM_Y);
			update(map2, rank, DIM_Y, row, size);
		}
	}

	//File output
	if((ITERNUM-1) % 2){
		printOut(map1, DIM_Y, rank, row, size);
	} else {
		printOut(map2, DIM_Y, rank, row, size);
	}
	MPI_Finalize();
	return 0;
}

void update(float map[][DIM], int rank, int DIM_Y, MPI_Datatype row, int size){
	int tag = 1;
	float topArr[DIM];
	float botArr[DIM];

	MPI_Status stat;

	if(rank == 0){
		MPI_Send(&map[DIM_Y-2][0], 1, row, (rank+1), tag, MPI_COMM_WORLD);
		MPI_Recv(botArr, DIM, MPI_FLOAT, (rank+1), tag, MPI_COMM_WORLD, &stat);
		for(int i = 0; i < DIM; i++){
			map[DIM_Y-1][i] = botArr[i];
		}
	}else if(rank == (size-1)){
		MPI_Recv(topArr, DIM, MPI_FLOAT, (rank-1), tag, MPI_COMM_WORLD, &stat);
		MPI_Send(&map[1][0], 1, row, (rank-1), tag, MPI_COMM_WORLD);
		for(int i = 0; i < DIM; i++){
			map[0][i] = topArr[i];
		}
	} else {
		MPI_Recv(topArr, DIM, MPI_FLOAT, (rank-1), tag, MPI_COMM_WORLD, &stat);
		MPI_Send(&map[1][0], 1, row, (rank-1), tag, MPI_COMM_WORLD);
		MPI_Send(&map[DIM_Y-2][0], 1, row, (rank+1), tag, MPI_COMM_WORLD);
		MPI_Recv(botArr, DIM, MPI_FLOAT, (rank+1), tag, MPI_COMM_WORLD, &stat);
		for(int i = 0; i < DIM; i++){
			map[0][i] = topArr[i];
		}
		for(int i = 0; i < DIM; i++){
			map[DIM_Y-1][i] = botArr[i];
		}
	}
}

void calcNew(float new[][DIM], float old[][DIM], const int DIM_Y){
	for(int i = 1; i < DIM_Y-1; i++){
		for(int j = 1; j < DIM-1; j++){
			new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
		}
	}
}

void printOut(float out[][DIM], const int DIM_Y, int rank, MPI_Datatype row, int size){
	//Gathering all Segmented parts of pixel map
	int tag = 1;
	MPI_Status stat;
	float outMapFull[DIM][DIM];
	if(rank != 0){
		if(rank == size-1){
			for(int i = 1; i < DIM_Y; i++){
				MPI_Send(&out[i][0], 1, row, 0, tag, MPI_COMM_WORLD);
			}
		}else{
			for(int i = 1; i < DIM_Y-1; i++){
				MPI_Send(&out[i][0], 1, row, 0, tag, MPI_COMM_WORLD);
			}
		}
	}

	if(rank == 0){
		for(int i = 0; i < DIM_Y; i++){
			for(int j = 0; j < DIM; j++){
				outMapFull[i][j] = out[i][j];
			}
		}

		int midCells = floor((double)DIM/(double)size);
		int endCell = ceil((double)DIM/(double)size) + (DIM % size);
	
		float tempArr[DIM];
		int rowNum = DIM_Y-1;
		bool reset = false;
		int resetInt = 1;
		for(int i = 1; i < size; i++){
			while(!reset && rowNum <= DIM){
				MPI_Recv(tempArr, DIM, MPI_FLOAT, i, tag, MPI_COMM_WORLD, &stat);
				for(int a = 0; a < DIM; a++){
					outMapFull[rowNum][a] = tempArr[a];
				}
				if(i == size-1 && resetInt >= endCell-1){
					reset = true;
				} else if(i < size-1 && resetInt > midCells-1){
					reset = true;
				}
				resetInt++;
				rowNum++;
			}
			reset = false;
			resetInt = 1;
		}

		//File IO to PPM file
		FILE *fptr;
	
		fptr = fopen("out.ppm", "w");
	
		if (fptr == NULL) {
	        printf("Could not open file for write!");
	    }
	
	    fprintf(fptr, "P3\n");
	    fprintf(fptr, "%d %d\n", DIM, DIM);
	    fprintf(fptr, "255\n");
	
	    //color handling 
	    for (int i = 0; i < DIM; i++) {
			for (int j = 0; j < DIM; j++) {
				if(outMapFull[i][j] > 250.0){
					fprintf(fptr, "255 0 0");
				} else if (outMapFull[i][j] > 180.0) {
					fprintf(fptr, "255 165 0");
				} else if (outMapFull[i][j] > 120.0) {
					fprintf(fptr, "255 255 0");
				} else if (outMapFull[i][j] > 80.0) {
					fprintf(fptr, "102 255 102");
				} else if (outMapFull[i][j] > 60.0) {
					fprintf(fptr, "0 255 0");
				} else if (outMapFull[i][j] > 50.0) {
					fprintf(fptr, "102 202 255");
				} else if (outMapFull[i][j] > 40.0) {
					fprintf(fptr, "0 0 255");
				} else if (outMapFull[i][j] > 30.0) {
					fprintf(fptr, "0 100 100");
				} else if (outMapFull[i][j] > 20.0) {
					fprintf(fptr, "51 25 0");
				} else {
					fprintf(fptr, "0 0 0");
				}
				fprintf(fptr, " ");
			}
			fprintf(fptr, "\n");
		}
		fclose(fptr);
	}
}
