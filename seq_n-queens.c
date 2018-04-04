#include "seq_n-queens.h" 

int numSol = 0; 

void printSolution(int board[][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf(" %d ", board[i][j]);
        printf("\n");
    }
    printf("\n");
}
 

bool isSafe(int board[N][N], int row, int col)
{
    int i, j;
 
    /* Check this row on left side */
    for (i = 0; i < col; i++)
        if (board[row][i])
            return false;
 
    /* Check upper diagonal on left side */
    for (i=row, j=col; i>=0 && j>=0; i--, j--)
        if (board[i][j])
            return false;
 
    /* Check lower diagonal on left side */
    for (i=row, j=col; j>=0 && i<N; i++, j--)
        if (board[i][j])
            return false;
 
    return true;
}
 
bool recursiveQueenPlacement(int board[N][N], int col)
{
    // Count each solution twice exept for middle row if odd board
    if (col >= N) {
      if(N % 2 == 1){
        if(board[N/2][0] == 0 ){
          numSol++;  
        }
      } else {
        numSol++;
      }

    	numSol++;

 		  return true;
    }

    //Place queen for each row in column
    for (int i = 0; i < N; i++)
    {
        // if legit placement
        if ( isSafe(board, i, col) )
        {
            //place queen
            board[i][col] = 1;
 
            // recur to place queen next column
            recursiveQueenPlacement(board, col + 1);
 
            board[i][col] = 0; 
        }
    }

    return false;
}

int main(int argc, char* argv[]) {

	n = strtol(argv[1], NULL, 10);
	int problems = N/2 + N%2;

	int boards[problems][N][N];

	for(int i = 0; i < problems; i++){
	    for(int j = 0; j < N; j++){
	    	for(int k = 0; k < N; k++){
	        	boards[i][j][k] = 0;
	    	}
	    }
	}

	for(int i = 0; i < problems; i++){
	   	boards[i][i][0] = 1;
	}

	for(int i = 0; i < problems; i++) {
		recursiveQueenPlacement(boards[i], 1);
	}

	printf("%d\n", numSol);
	   
	return 0;
}