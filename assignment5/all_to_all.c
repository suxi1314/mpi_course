/*
* Done
* 用MPI_SEND和MPI_RECV实现MPI_ALLTOALL程序
* compile
* mpicc -o all_to_all all_to_all.c -no-pie
* run
* mpirun -np 3 ./all_to_all


0 1 2
3 4 5
6 7 8

0 3 6
1 4 7
2 5 8

* 要注意的是不能发消息给自己
*/
#include <mpi.h>  
#include <stdio.h>  
#include <math.h>  

/*Start the main program*/
void main(argc, argv)
int argc; char **argv;
{
	int iam, np; // iam是进程号，表示我是哪个进程
	MPI_Comm comm; 
    int i;
    int a[3];
    int b[3];
    MPI_Status st;

	/*Start MPI environment*/
	MPI_Init( &argc, &argv ); // 初始化mpi进程，其他函数都要在该函数后调用
	MPI_Comm_dup( MPI_COMM_WORLD, &comm ); //MPI_COMM_WORLD是全局通信子, dup是复制操作, 把MPI_COMM_WORLD复制成comm，好用且防止破坏
	MPI_Comm_rank( comm, &iam );
	MPI_Comm_size( comm, &np );

    a[0] = iam*3+0;
    a[1] = iam*3+1;
    a[2] = iam*3+2;

    if(iam == 0){
        b[iam] = a[iam];
        MPI_Send(a+1, 1, MPI_INT, 1, iam, comm);
        MPI_Send(a+2, 1, MPI_INT, 2, iam, comm);
    
        for(int i = 0; i < 3; i ++){
            if(i != iam){
                MPI_Recv(b+i, 1, MPI_INT, i, i, comm, &st);
            }
        }
        printf("The process %d of %d is running! receive %d %d %d.\n", iam, np, b[0], b[1], b[2]);
        
    }



    if(iam == 1){
        b[iam] = a[iam];
        MPI_Send(a+0, 1, MPI_INT, 0, iam, comm);
        MPI_Send(a+2, 1, MPI_INT, 2, iam, comm);
        for(int i = 0; i < 3; i ++){
            if(i != iam){
                MPI_Recv(b+i, 1, MPI_INT, i, i, comm, &st);
            }
        }
        printf("The process %d of %d is running! receive %d %d %d.\n", iam, np, b[0], b[1], b[2]);
        
    }


    if(iam == 2){
        b[iam] = a[iam];
        MPI_Send(a+0, 1, MPI_INT, 0, iam, comm);
        MPI_Send(a+1, 1, MPI_INT, 1, iam, comm);
        for(int i = 0; i < 3; i ++){
            if(i != iam){
                MPI_Recv(b+i, 1, MPI_INT, i, i, comm, &st);
            }
        }
        printf("The process %d of %d is running! receive %d %d %d.\n", iam, np, b[0], b[1], b[2]);
        
    }


	/*finished*/
	MPI_Finalize( );
}
