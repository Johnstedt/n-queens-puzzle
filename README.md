# Tremory webb and desktop repository

**Clone and run for a quick way to see it.**

This currently doesnt work on desktop due to the tree requiring heavy DOM manipulation.

## Requirements

The mpicc compiler and a system supporting MPI is required for the parallel
program.

## To Use

```bash
# Clone this repository
git clone https://github.com/Johnstedt/N_Queen_Puzzle.git
# Go into the repository
cd TremoryDesktop
# add dependencies
git clone https://github.com/Johnstedt/Timing_Statistics_Utils.git
# Compile parallel
make n-queens
# Compile sequential
make seq_n-queens

#run parallel
mpirun -n <threads> ./n-queens <board size>

#run sequential
./seq_n-queens <board size>

```

