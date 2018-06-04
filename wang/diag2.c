/*
 * compile
 * mpicc -o  diag2 diag2.c -no-pie -Wall
 * run
 * mpirun -np 2 ./diag2

P145
N*N分块下三角矩阵的对角线上的块都是M*M阶,
给出下三角矩阵的数据类型用于传输对角块

# # 0 0 0 0   
# # 0 0 0 0
* * # # 0 0
* * # # 0 0
* * * * # # 
* * * * # #


0  1  0  0  0  0 
6  7  0  0  0  0 
0  0 14 15  0  0 
0  0 20 21  0  0 
0  0  0  0 28 29
0  0  0  0 34 35


D0  0   0

A   D1  0

B   C   D2

对角块D1上的元素 

可以先构造传递一个块的数据类型nt
再用nt组成所有对角的数据数据类型fnt

*/

#include <mpi.h>  
#include <stdio.h>  
#include <math.h>  
#define N 6  //方阵行数
#define M 2  //块的阶数
#define K N/M //块数
/*Start the main program*/
int main(argc, argv)
int argc; char **argv;
{

	int iam, np; // iam是进程号，表示我是哪个进程

	MPI_Comm comm; 
    int a[N*N];

    int i, j;
    MPI_Datatype nt, fnt;

    int tag = 1;
    MPI_Status st;

	// Start MPI environment
	MPI_Init( &argc, &argv ); // 初始化mpi进程，其他函数都要在该函数后调用
	MPI_Comm_dup( MPI_COMM_WORLD, &comm ); //MPI_COMM_WORLD是全局通信子, dup是复制操作, 把MPI_COMM_WORLD复制成comm，好用且防止破坏
	MPI_Comm_rank( comm, &iam );
	MPI_Comm_size( comm, &np );

    // 一个块的数据类型nt
    MPI_Type_vector(M, M, N, MPI_INT, &nt);
    MPI_Type_commit(&nt);

    MPI_Aint extent;
    MPI_Type_extent(MPI_INT, &extent);

    // 所有块的数据类型fnt
    MPI_Type_create_hvector(K, 1, (M*N+M) * extent, nt, &fnt);
    MPI_Type_commit(&fnt);


    if(iam == 0) 
    { 
        for(i = 0; i < N*N; i++) 
            a[i] = i;
        MPI_Send(a, 1, fnt, 1, tag, comm);

        for(i = 0; i < N; i++) 
            for(j = 0;j < N; j++)
             printf("a[%d]=%d ", i*N+j, a[N*i+j]);
        printf("\n");

    }
    else if(iam == 1) 
    {
        for(i = 0; i < N*N; i++) 
            a[i] = 0;
        MPI_Recv(a, 1, fnt, 0, tag, comm, &st);
        printf("The process %d of %d is running!\n", iam, np); 

        for(i = 0; i < N; i++){
            for(j = 0;j < N; j++)
                printf("a[%d]=%d ", i*N+j, a[N*i+j]);
            printf("\n");
        }

    }

    MPI_Type_free(&nt);
    MPI_Type_free(&fnt);

	// finished
	MPI_Finalize();

    return 0;
}
