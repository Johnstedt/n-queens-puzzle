#include <stdio.h>
#include <stdlib.h>

#define N n
typedef enum { false, true } bool;

int n;

void printSolution(int board[][N]);

bool isSafe(int board[N][N], int row, int col);
 
bool solveNQUtil(int board[N][N], int col);

bool solveNQ(int startBoard[N][N], int depth);

int calculateSufficientDepth(int threads, int boardSize);

int getProblems(int depth, int boardSize);

int power(int x, int y);