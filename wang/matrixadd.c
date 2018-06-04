/*在主程序中定义了矩阵A和B(比如A[50][67],B[73][49])，
 *请写一个通用子程序完成矩阵相加，使得对于任何m*n的
 *子矩阵A和子矩阵B相加都能调用该子程序
 *(比如matadd(m,n,...,A...,B...,C))
gcc -Wall -o matrixadd.o matrixadd.c 
 */
#include <stdio.h>
#include <stdlib.h>
//二维数组
int matadd_2division(int m, int n, int lda, int (* A)[lda], int ldb, int (* B)[ldb], int (* C)[n])
{
   
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return 0;
}
//一维数组
int matadd_1division(int m, int n, int lda, int A[], int ldb, int B[], int C[]){
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            C[i*n+j] = A[i*lda+j]+B[i*ldb+j];
    return 0;
}

int main(void)
{
    int A[100][100];
    int B[100][100];
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            A[i][j] = i;
            B[i][j] = j;
        }
    }
    int C[3][4];
    matadd_2division(3, 4, 100, A, 100, B, C);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 4; j++)
            printf("A[i][j] = %d, B[i][j] = %d, C[i][j] = %d\n", A[i][j], B[i][j] , C[i][j]);



    int D[12];
    matadd_1division(3, 4, 100, A[0], 100, B[0], D);
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 4; j++)
		    printf("%d, %d, %d\n", A[0][i*100+j], B[0][i*100+j] , D[i*4+j]);
    return 0;
}


