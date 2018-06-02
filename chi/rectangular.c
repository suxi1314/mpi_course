#include "myhead.h"
/*for float data*/

void rectangular( m, n, lda, pos, matrix )
int m, n, lda;
MPI_Aint pos;
MPI_Datatype *matrix;
{
  MPI_Datatype temp, dtp[2];
  int blk[2];
  MPI_Aint dsp[2];

  MPI_Type_vector( m, n, lda, MPI_FLOAT, \
                   &temp );  

  blk[0] = 1; blk[1] = 1;
  dsp[0] = 0; dsp[1] = pos;
  dtp[0] = temp; dtp[1] = MPI_UB;

  MPI_Type_struct( 2, blk, dsp, dtp, matrix);
  MPI_Type_extent( temp, &dsp[0] );
  MPI_Type_extent( *matrix, &dsp[1] );
  printf("\nThe extents are : %d and %d \n", dsp[0], dsp[1]);

  return;
}
