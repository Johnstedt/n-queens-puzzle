n-queens: n-queens.c
	mpicc.openmpi $? -O3 -o $@

seq_n-queens: seq_n-queens.c
	gcc $? -O3 -o $@

speed: speedUp.c Timing_Statistics_Utils/timath.c 
	mpicc.openmpi $? -lm -o $@