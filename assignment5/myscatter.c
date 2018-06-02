/* 
 * done
 * compile
 * mpicc -o myscatter myscatter.c -no-pie -Wall
 * run
 * mpirun -np 3 ./myscatter

implement a scatter
scatter 从一个进程分发连续数据块到所有进程
   c = {0,1,2,3,4, //把数据d0发给p0
        5,6,7,8,9, //把数据d1发给p1
        10...
       } 
5个进程p 0 1 2 3 4，从p0分发数据c

                                         p0:d0
                    p0(p1):d0,d1       ->
                                         p1:d1
p0:d0,d1,d2,d3,d4 -> 
                                        
                    p2(p3,p4):d2,d3,d4 ->p2:d2           p3:d3
                                         p3(p4):d3,d4  ->
                                                         p4:d4
二分法，每次把进程划分为两个进程组，从源进程发送后一半数据给另一个进程组中的第一个进程，该进程成为新的源，再次分发


 */
#include <mpi.h>  
#include <stdio.h>   

void scatter(comm, blk, a)
MPI_Comm comm;
int blk;
float *a;
{
    int iam, np, len, color, key;
    MPI_Status st;
    MPI_Comm tcom, scom;

    MPI_Comm_size(comm, &np);
    MPI_Comm_rank(comm, &iam);

    MPI_Comm_dup(comm, &tcom);
    


    while(np > 1){
        len = np/2; //每块长度为blk, 从len块发出，也就是二分后的第一个进程
        // 从此时的源进程0发给了另一半进程中的第一个，发送的数据是后一半
        if(0 == iam) MPI_Send(&a[len * blk], (np -len)*blk, MPI_FLOAT, len, 1, tcom);
        if(len == iam) MPI_Recv(&a[0], (np -len)*blk, MPI_FLOAT, 0, 1, tcom, &st);

        if(iam < len) color = 0;
        else color = 1;
        key = iam;
        MPI_Comm_split(tcom, color, key, &scom);
        MPI_Comm_dup(scom, &tcom);
        MPI_Comm_size(tcom, &np);
        MPI_Comm_rank(tcom, &iam);
    }

    return;
}

/*Start the main program*/
int main(argc, argv)
int argc; char **argv;
{
	int iam, np; // iam是进程号，表示我是哪个进程
	MPI_Comm comm; 

    float c[40];
    int i;


	/*Start MPI environment*/
	MPI_Init( &argc, &argv ); // 初始化mpi进程，其他函数都要在该函数后调用
	MPI_Comm_dup( MPI_COMM_WORLD, &comm ); //MPI_COMM_WORLD是全局通信子, dup是复制操作, 把MPI_COMM_WORLD复制成comm，好用且防止破坏
	MPI_Comm_rank( comm, &iam );
	MPI_Comm_size( comm, &np );


	if(iam == 0){
        for(i = 0; i<40; i++) 
            c[i] = i;
    }else{
        for(i = 0; i<40; i++) 
            c[i] = 0;
    }

    scatter(comm, 5, &c[0]);

	/*MAIN work here */
	printf("The process %d of %d is running!c[0]=%f,c[5]=%f,c[9]=%f.\n", 
            iam, np, c[0],c[5],c[9]); 
	/*finished*/
	MPI_Finalize( );
    return 0;
}
