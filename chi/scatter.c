#include "myhead.h"

void scatter( comm, blk, a )
MPI_Comm comm;
int blk;
float *a;
{
  int iam, np;
  MPI_Comm tcom, scom;
  MPI_Status st;
  int len, color, key;

  MPI_Comm_size( comm, &np );
  MPI_Comm_rank( comm, &iam );
  MPI_Comm_dup( comm, &tcom );

  while ( np > 1 ) {
    len = np / 2;
    if( iam == 0 ) MPI_Send( &a[len*blk], \
      (np-len)*blk, MPI_FLOAT, len, 1, tcom );
    if( iam == len ) MPI_Recv( &a[0], \
      (np-len)*blk, MPI_FLOAT, 0, 1, tcom, &st );
    if ( iam < len ) color = 0;
    else color = 1;
    key = iam;
    MPI_Comm_split( tcom, color, key, &scom );

    MPI_Comm_dup( scom, &tcom ); 
    MPI_Comm_size( tcom, &np );
    MPI_Comm_rank( tcom, &iam );
  }

  return;
}
