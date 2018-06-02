#include "myhead.h"

void copy( a, lda, b, ldb, m, n )
float *a, *b;
int lda, ldb, m, n;
{
  int i, j;
  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      b[i*ldb+j] = a[i*lda+j];

  return;
}

void sgemm( a, lda, b, ldb, c, ldc, m, k, n )
float *a, *b, *c;
int lda, ldb, ldc, m, k, n;
{
  int i, j, l;

  for( i=0; i<m; i++ )
    for( j=0; j<n; j++ )
      for( l=0; l<k; l++ )
        c[i*ldc+j] += a[i*lda+l]*b[l*ldb+j];

  return;
}

void setc( c, ldc, m, n )
float *c;
int ldc, m, n;
{
  int i, j;

  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      c[i*ldc+j] = 0.0;

  return;
}


void seta( c, ldc, m, n, rid, cid )
float *c;
int ldc, m, n, rid, cid;
{
  int i, j, offi, offj;
  offi = rid*m;
  offj = cid*n;
  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      c[i*ldc+j] = offi+offj+i+j;

  return;
}

void setb( c, ldc, m, n, rid, cid )
float *c;
int ldc, m, n, rid, cid;
{
  int i, j, offi, offj;

  offi = rid*m;
  offj = cid*n;

  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      c[i*ldc+j] = 1.0- 2*((offi+offj+i+j)%2);

  return;
}

void cannon(rowcom, colcom, lda, a, ldb, b, \
            ldc, c, m, k, n, wk)
MPI_Comm rowcom, colcom;
int lda, ldb, ldc, m, k, n;
float *a, *b, *c, *wk;
{
  int rid, cid, np, l, i, nxt, frnt;
  MPI_Datatype dta, dtb;
  MPI_Status st;

  MPI_Comm_rank( rowcom, &cid );
  MPI_Comm_rank( colcom, &rid );
  MPI_Comm_size( colcom, &np );

  nxt = (np+rid-1)%np;
  frnt = (rid+1)%np;

  MPI_Type_vector( m, k, lda, MPI_FLOAT, \
                   &dta );
  MPI_Type_vector( k, n, ldb, MPI_FLOAT, \
                   &dtb );

  MPI_Type_commit( &dta );
  MPI_Type_commit( &dtb );

  for( i=0; i<np; i++ ) {
    l = (i + rid)%np;
    if( cid == l ) copy( a, lda, wk, lda, m, k );
    MPI_Bcast( wk, 1, dta, l, rowcom );

    sgemm( wk, lda, b, ldb, c, ldc, m, k, n);
    if( i == np-1 ) break;
    MPI_Sendrecv( b, 1, dtb, nxt, 1, wk, 1, \
                  dtb, frnt, 1, colcom, &st);
    copy( wk, ldb, b, ldb, k, n );
  }

  MPI_Type_free( &dta );
  MPI_Type_free( &dtb );

  return;
}
