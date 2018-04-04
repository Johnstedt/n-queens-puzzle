#include "n-queens.h" 

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

int calculateSufficientDepth(int threads, int boardSize) {

  if(threads*5 <= ((boardSize/2) + (boardSize % 2))){
    return 1;
  } else if (threads*5 <= ((boardSize/2)+(boardSize % 2))*boardSize) {
    return 2;
  }
  else { //depth beyond three is ridiculous in all but the most trivial board sizes.
    return 3;
  }
}

int getProblems(int depth, int boardSize){
  if(depth == 1){
    return boardSize/2 + (boardSize % 2) ;
  } 
  else if (depth == 2){
    return ((boardSize/2)+(boardSize % 2))*boardSize;
  } 
  else {
    return ((boardSize/2)+(boardSize % 2))*boardSize*boardSize;
  }
}

int power(int x, int y)
{
  int sum = x;
  for(int i = 0; i < y-1; i++){
    sum = sum*x;
  }
  return sum;
}


int main(int argc, char* argv[]) {

  int my_rank, comm_sz;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  n = strtol(argv[1], NULL, 10);;

  int requiredDepth = calculateSufficientDepth(comm_sz, N);
  int problems = getProblems(requiredDepth, N);

  int boards[problems][N][N];

  //printf(" Depth %d, Problems %d \n", requiredDepth, problems );

  for(int i = 0; i < problems; i++){
    for(int j = 0; j < N; j++){
      for(int k = 0; k < N; k++){
        boards[i][j][k] = 0;
      }
    }
  }

  for(int i = 0; i < problems; i++){
    for(int j = 0; j < (requiredDepth); j++){
      
      if(j == requiredDepth-1)
      {   
        if(isSafe(boards[i], (i % N), j))
        {
          boards[i][i % N][j] = 1;
        }
        else 
        {
          boards[i][i % N][j] = 1;
          boards[i][0][0] = -1;
        }
      } else 
      {

        if(isSafe(boards[i], (i/ ((requiredDepth - 1 - j) * N) % N), j)){
          boards[i][(i/ power(N, (requiredDepth -1- j) )) % N][j] = 1;
        }
        else 
        {
          boards[i][(i/ power(N, (requiredDepth -1- j) )) % N][j] = 1;
          boards[i][0][0] = -1;
        }
      }   
         
    }
  }

  int *gather;
  gather = calloc(N, sizeof(int));

  int *apointer;
  apointer = calloc(N, sizeof(int));

  int *recvbuf;
  recvbuf = calloc(1, sizeof(int));

  MPI_Status stat;

  if(my_rank != 0){
    bool quit = false;
    
    while(!quit) 
    {
      int someval = 0;

      MPI_Send(&someval, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

      //Also suppose that process r calls MPI Recv with
      MPI_Recv(recvbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

      if(*recvbuf == -1)
      {
        
        quit = true;
      } else if ( *recvbuf == (problems -1) && boards[*recvbuf][0][0] != -1){
        recursiveQueenPlacement(boards[*recvbuf], requiredDepth);
        
      }
      else 
      {
        if(boards[*recvbuf][0][0] != -1)
        {
          recursiveQueenPlacement(boards[*recvbuf], requiredDepth);
        }
        
      }
    }

  } else 

  {
    
    int problem = 0;
    while(problem < problems){

      MPI_Recv(recvbuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
    
      // answer 
      MPI_Send(&problem, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
      
      problem++;
    }

    int shutup = 0;
    int shutit = -1;
    while(shutup < comm_sz-1)
    {
      MPI_Recv(recvbuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
    
      // answer to process `stat.MPI_SOURCE` using `someotherval` as tag
      MPI_Send(&shutit, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);

      shutup++;
    }

  }

  apointer[0] = numSol;

  MPI_Reduce(apointer, gather, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(my_rank == 0)
  {
    printf("%d\n", *gather);
  }
  free(gather);
  free(apointer);

  MPI_Finalize(); 
  return 0;
}