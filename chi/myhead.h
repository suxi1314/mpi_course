#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

void begin( int*, char***, MPI_Comm*, int*, int*);
void end( );
void hello( int, int );
void rectangular(int, int, int, MPI_Aint, MPI_Datatype *);
void bcast( float *, int, int, MPI_Comm );
void mesh( int, int, MPI_Comm, int *, int *, \
           MPI_Comm *, MPI_Comm *, int, int );
void setc( float *, int, int, int);
void seta( float *, int, int, int, int, int);
void setb( float *, int, int, int, int, int);
void cannon( MPI_Comm, MPI_Comm, int, float *, \
             int, float *, int, float *, int, \
             int, int, float * );
void scatter( MPI_Comm, int, float * );           
void matvec( float *, int, float *, float *, \
             float *, int, int, MPI_Comm, int, \
             int, float * );
