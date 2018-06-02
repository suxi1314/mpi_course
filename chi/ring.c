#include "myhead.h"

void ring( m, iam, np, comm )
int *m, iam, np;
MPI_Comm comm;
{
  int n=*m, next, front;
  MPI_Status st;

  next = (iam+1) % np;
  front = (iam-1 +np) % np;

  if( iam == 0 ) {
    MPI_Recv( m, 1, MPI_INT, front, 1, comm, &st );
    MPI_Send( &n, 1, MPI_INT, next, 1, comm );
  }
  else {
    MPI_Send( &n, 1, MPI_INT, next, 1, comm );
    MPI_Recv( m, 1, MPI_INT, front, 1, comm, &st );
  }

  return;
}
