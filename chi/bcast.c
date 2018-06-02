#include "myhead.h"

void bcast( a, cnt, root, comm )
float *a;
int cnt, root;
MPI_Comm comm;
{
  int iam, np;
  int next, front;
  MPI_Status st;
  int n, newid, m, i, num;

  MPI_Comm_size( comm, &np );
  MPI_Comm_rank( comm, &iam );

  newid = (np+iam-root)%np;

  n = log(np+0.001) / log( 2.0 );
  num = 1;
  for(i=0; i<n; i++) {
    if ( newid < num ) {
      next = (iam + num)%np;
      MPI_Send( a, cnt, MPI_FLOAT, next, \
                1, comm );
    } 
    else if ( newid < 2* num ) {
      front = (np+iam-num)%np;
      MPI_Recv( a, cnt, MPI_FLOAT, front, \
                1, comm, &st );
    }
    num = 2*num;
  }
  m = np-num;
  if ( m != 0 ) {
    if ( newid < m ) {
      next = (iam + num)%np;
      MPI_Send( a, cnt, MPI_FLOAT, next, \
                1, comm );
    }
    else if ( newid >= num ) {
      front = (np+iam-num)%np;
      MPI_Recv( a, cnt, MPI_FLOAT, front, \
                1, comm, &st );
    }
  }
}
