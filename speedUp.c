#include "Timing_Statistics_Utils/timath.h"
#include <wait.h>
#include <unistd.h>
#include <string.h>

double runWithBoardSize(int bz, int th)
{

	double time[5];
	double start;
	double stop;


	char size[2];
	char threads[2];

    sprintf(size, "%d", bz);
    sprintf(threads, "%d", th);

    char* fileName = malloc(50);

    

    if(th == 1){
    	strcat(fileName, " ./seq_n-queens ");	
    } else {
    	strcpy(fileName, "mpirun -n ");
    	strcat(fileName, threads);
    	strcat(fileName, " ./n-queens ");
	}
    strcat(fileName,  size);
    strcat(fileName, " >> data.txt");

    //printf("%s", fileName);
    //printf("\n");

	for(int i = 0; i < 5; i++){
		start = getWallTime();

		system(fileName);

		stop = getWallTime();
		time[i] = stop - start;

		printf(" %d & %d & %lf  \\\\hline\n", bz, th, time[i]);
	}

/*	
	printf("\n --- EXPERIMENT WITH %d THREADS AND %d x %d BOARD --- \n\n",  th, bz, bz);
	printf("MEDIAN: %lf\n", median(5, time));
	printf("MEAN: %lf\n", mean(5, time));
	printf("VARIANCE: %lf\n", variance(5, time));
	printf("DEVIATION: %lf\n", deviation(5, time));
	printf("\n\n --- EXPERIMENT OVER --- \n\n");
*/	

	printf(" %d & %d & %lf & %lf & %lf & %lf \\hline \n", bz, th, median(5, time), mean(5, time), variance(5, time), deviation(5,time));

	return median(5, time);
}




int main(int argc, char const *argv[])
{

	//int th = strtol(argv[1], NULL, 10);

	int th = 8;

	// PROBLEM SIZE CHANGE

	double problemSize[15];

	problemSize[0] = runWithBoardSize(6 ,th);
	problemSize[1] = runWithBoardSize(8, th);
	problemSize[2] = runWithBoardSize(10, th);
	problemSize[3] = runWithBoardSize(12, th);


	// THREAD SIZE CHANGE

	double threadSize[15];

	threadSize[0] = runWithBoardSize(14 ,2);
	threadSize[1] = runWithBoardSize(14, 4);
	threadSize[2] = runWithBoardSize(14, 6);
	threadSize[3] = runWithBoardSize(14, 8);

	printf("\n problem size \n\n");

	for(int i = 0; i < 4; i++){
		printf("(%d, %lf)\n", i*2 + 6, problemSize[i]);
	}

	printf("\n\n thread size \n\n");

	for(int i = 0; i < 4; i++){
		printf("(%d, %lf)\n", 2+i*2, threadSize[i]);
	}

	printf("\n\n");

	/* code */
	return 0;
}