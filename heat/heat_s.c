#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

const int DIM = 1000;

void calcNew(float new[][DIM], float old[][DIM], const int DIM);

void printOut(float out[][DIM], const int DIM);

int main(int argc, char *argv[]) {

	//Heat Code
	const int FIREWIDTH = ((int)(DIM * 0.4)) - (((int)(DIM * 0.4)) % 2);
	const int ITERNUM = 10000;

	float map1[DIM][DIM];
	float map2[DIM][DIM];

	//Ambient Temp
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			map1[i][j] = 20.0;
			map2[i][j] = 20.0;
		}
	}

	//Fireplace
	for(int i = 0; i < FIREWIDTH; i++){
		map1[0][((DIM/2)-FIREWIDTH/2) + i] = 300.0;
		map2[0][((DIM/2)-FIREWIDTH/2) + i] = 300.0;
	}

	//Loop for simulation
	for (int i = 0; i < ITERNUM; ++i) {
		if(i % 2 == 0){
			calcNew(map1, map2, DIM);
		} else {
			calcNew(map2, map1, DIM);
		}
	}

	//File output
	if((ITERNUM-1) % 2){
		printOut(map1, DIM);
	} else {
		printOut(map2, DIM);
	}

	return 0;
}

void calcNew(float new[][DIM], float old[][DIM], const int DIM){
	for(int i = 1; i < DIM-1; i++){
		for(int j = 1; j < DIM-1; j++){
			new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
		}
	}
}

void printOut(float out[][DIM], const int DIM){
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
			if(out[i][j] > 250.0){
				fprintf(fptr, "255 0 0");
			} else if (out[i][j] > 180.0) {
				fprintf(fptr, "255 165 0");
			} else if (out[i][j] > 120.0) {
				fprintf(fptr, "255 255 0");
			} else if (out[i][j] > 80.0) {
				fprintf(fptr, "102 255 102");
			} else if (out[i][j] > 60.0) {
				fprintf(fptr, "0 255 0");
			} else if (out[i][j] > 50.0) {
				fprintf(fptr, "102 202 255");
			} else if (out[i][j] > 40.0) {
				fprintf(fptr, "0 0 255");
			} else if (out[i][j] > 30.0) {
				fprintf(fptr, "0 100 100");
			} else if (out[i][j] > 20.0) {
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