/*

* compile
* mpicc -o matvec matvec.c -no-pie -Wall
* run
* mpirun -np 3 ./matvec
y=Ax+b

A和x均按行分块，每行块存储在同一个process里面，每块m行
A的每行又按列分块，每块n列

仍然是对角线

a00 a01    a02 a03    a04 a05   x0   b0   y0
a10 a11    a12 a13    a14 a15   x1   b1   y1

a20 a21    a22 a23    a24 a25   x2   b2   y2
a30 a31    a32 a33    a34 a35   x3   b3   y3

a40 a41    a42 a43    a44 a45   x4   b4   y4
a50 a51    a52 a53    a54 a55   x5   b5   y5



p0: A00    A01     A02      X0   B0   Y0 

p1: A10    A11     A12      X1   B1   Y1

p2: A20    A21     A22      X2   B2   Y2


A00 X0 + A01 X1 + A02 X2 + B0  = Y0 
p0  p0   p0  p1   p0  p2   p0    p0

A10 X0 + A11 X1 + A12 X2 + B1  = Y1
p1  p0   p1  p1   p1  p2   p1    p1

A20 X0 + A21 X1 + A22 X2 + B2  = Y2
p2  p0   p2  p1   p2  p2   p2    p2


先算在同一个p中的块，再循环传递X

A00 X0 + A01 X1 + A02 X2 + B0  = Y0 
p0  p0   p0  p1   p0  p0   p0    p0

A10 X0 + A11 X1 + A12 X2 + B1  = Y1
p1  p1   p1  p1   p1  p2   p1    p1

A20 X0 + A21 X1 + A22 X2 + B2  = Y2
p2  p0   p2  p2   p2  p2   p2    p2


再算在同一个p中的块，再循环传递X

A00 X0 + A01 X1 + A02 X2 + B0  = Y0 
p0  p0   p0  p0   p0  p0   p0    p0

A10 X0 + A11 X1 + A12 X2 + B1  = Y1
p1  p1   p1  p1   p1  p1   p1    p1

A20 X0 + A21 X1 + A22 X2 + B2  = Y2
p2  p2   p2  p2   p2  p2   p2    p2

算在同一个p中的块...


*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

// lda为a的列数
void sgemv(a, lda, x, y, m, n)
int *a, *x, *y;
int m, n, lda;
{
   int i, j;
   for( i=0; i<m; i++)
       for( j=0; j<n; j++ )
           y[i] += a[i*lda+j]*x[j];
   return;
}


// Ax+b->y
void matvec(a, lda, x, b, y, m, n, comm, iam, np, wk)
int *a, *x, *b, *y, *wk;
int lda, m, n, iam, np;
MPI_Comm comm;
{
    int i, j, l, frnt, nxt;
    MPI_Status st;

    frnt = (iam + 1)%np;
    nxt = (np+iam-1)%np;    
    
    for( i=0; i<n; i++ )   y[i]=b[i];

    for(i = 0; i < np; i++){
        l = (i + iam)%np;
        // y(m) = a(m*n) * x(m) 
        sgemv(&a[l*n], lda, x, y, m, n);
        MPI_Sendrecv(x, n, MPI_INT, nxt, 1, wk, n, MPI_INT, frnt, 1,comm, &st);
        for(j = 0; j<n; j++) x[j] = wk[j];
    }

    return;
    
}

/*Start the main program*/
int main(argc, argv)
int argc; char **argv;
{
	int iam, np; // iam是进程号，表示我是哪个进程
	MPI_Comm comm; 

    int i, lda, m, n;

    int a[100], x[50], b[50], y[50];
    int wk[50];


	/*Start MPI environment*/
	MPI_Init( &argc, &argv ); // 初始化mpi进程，其他函数都要在该函数后调用
	MPI_Comm_dup( MPI_COMM_WORLD, &comm ); //MPI_COMM_WORLD是全局通信子, dup是复制操作, 把MPI_COMM_WORLD复制成comm，好用且防止破坏
	MPI_Comm_rank( comm, &iam );
	MPI_Comm_size( comm, &np );

    // 假设a是方阵
    m = n = 2;
    lda = np * n;

    for(i = 0; i < m * lda; i++)
        a[i] = i;

    for(i = 0; i < n; i++)
    {
        x[i] = iam * n + i;
        b[i] = 1;
        y[i] = 0;
    }


    matvec(a, lda, x, b, y, m, n, comm, iam, np, wk);

	/*MAIN work here */
	printf("The process %d of %d is running!y[0]=%d,y[1]=%d\n", 
            iam, np, y[0], y[1]); 
	/*finished*/
	MPI_Finalize( );
    return 0;
}
