#include "myhead.h"


void main(argc, argv)
int argc;
char **argv;
{
  MPI_Comm comm;
  int np, iam;
  int i, m, k, n;
  float a[100][101], b[50][50], c[37][43];
  float wk[100][100];
  MPI_Datatype nt, fnt;
  MPI_Status st;
  MPI_Comm rcom, ccom;
  int rid, cid, p;

  begin(&argc, &argv, &comm, &np, &iam);

/*main body here*/
  if( iam == 0 ) \
    for ( i=0; i<40; i++ ) c[0][i] = i;
  scatter( comm, 5, &c[0][0] );
  printf( "\nVs are %f, %f, %f in %d\n",\
          c[0][0], c[0][1], c[0][2], iam );
/*terminate*/
  end();
}
