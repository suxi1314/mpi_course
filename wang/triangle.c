/*
 * compile
 * mpicc -o  triangle triangle.c -no-pie -Wall
 * run
 * mpirun -np 2 ./triangle

N*N分块下三角矩阵的块都是M*M阶,
给出传送下三角矩阵的数据类型

# # 0 0 0 0   
# # 0 0 0 0

# # # # 0 0
# # # # 0 0

# # # # # # 
# # # # # #


0  1  0  0  0  0 
6  7  0  0  0  0 
12 13 14 15  0  0 
18 19 20 21  0  0 
24 25 26 27 28 29
30 31 32 33 34 35


可以先构造传递一个块的数据类型nt
再构造传递第k行的数据类型
最后把k行组合成一个矩阵的数据类型

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

    int i, j, k;
    MPI_Datatype nt, fnt[K], mnt;

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

    MPI_Aint extent, lb;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

    int bln[K];
    MPI_Aint dsp[K];

    // 矩阵分块后第k行的数据类型
    for(k = 0; k < K; k++){
        MPI_Type_create_hvector(k+1, 1, M*extent, nt, &fnt[k]);
        MPI_Type_commit(&fnt[k]);
        bln[k] = 1;
        dsp[k] = k*M*N*extent;
    }

    MPI_Type_create_struct(K, bln, dsp, fnt, &mnt);
    MPI_Type_commit(&mnt);

    if(iam == 0) 
    { 
        for(i = 0; i < N*N; i++) 
            a[i] = i;
        MPI_Send(a, 1, mnt, 1, tag, comm);
/*
        for(i = 0; i < N; i++) 
            for(j = 0;j < N; j++)
             printf("a[%d]=%d ", i*N+j, a[N*i+j]);
        printf("\n");
*/

    }
    else if(iam == 1) 
    {
        for(i = 0; i < N*N; i++) 
            a[i] = 0;
        MPI_Recv(a, 1, mnt, 0, tag, comm, &st);
        printf("The process %d of %d is running!\n", iam, np); 

        for(i = 0; i < N; i++){
            for(j = 0;j < N; j++)
                printf("a[%d]=%d ", i*N+j, a[N*i+j]);
            printf("\n");
        }
    }

    for(k = 0; k < K; k++)
        MPI_Type_free(&fnt[k]);
    MPI_Type_free(&nt);
    MPI_Type_free(&mnt);

	// finished
	MPI_Finalize();

    return 0;
}
