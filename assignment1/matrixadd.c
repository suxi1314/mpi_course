/*在主程序中定义了矩阵A和B(比如A[50][67],B[73][49])，
 *请写一个通用子程序完成矩阵相加，使得对于任何m*n的
 *子矩阵A和子矩阵B相加都能调用该子程序
 *(比如matadd(m,n,...,A...,B...,C))
 */
#include <stdio.h>
#include <stdlib.h>
int printMatrix(int m, int n, int (* A)[n], const char *name);
//实际上A和B可以只取出m*n的部分传给函数
int matadd(int m, int n, int a0, int a, int (* A)[a], int b0, int b, int (* B)[b], int (* C)[n]);
int main(void)
{
    const int a0 = 4;
    const int a = 10;
    const int b0 = 5;
    const int b = 8;
    int A[a0][a];
    int B[b0][b];
    for(int i = 0; i < a0; i ++)
    {
        for(int j = 0; j < a; j++)
        {
            A[i][j] = i;
        }
    }
    for(int i = 0; i < b0; i ++)
    {
        for(int j = 0 ; j < b; j++)
        {
            B[i][j] = j;
        }
    }
    int m = 3;
    int n = 4;
    printf("Please enter size of add matrix m, n:");
    scanf("%d%d", &m, &n);
    int C[m][n];
    if(!matadd(m, n, a0, a, A, b0, b, B, C))
    {
        printMatrix(a0, a, A, "A");
        printMatrix(b0, b, B, "B");
        printMatrix(m, n, C, "C");
    }
    else
    {
        printf("Invalid m or n. Can't add!\n");
    }
}
int matadd(int m, int n, int a0, int a, int (* A)[a], int b0, int b, int (* B)[b], int (* C)[n])
{
    if((m<=a0)&&(n<=a)&&(m<=b0)&&(n<=b))
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
    else
		return 1;
}

int printMatrix(int m, int n, int (* A)[n], const char *name)
{
    FILE *stream;
    stream = fopen ("outfile.txt", "a");
    fprintf(stream,"%s=\n", name);
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            fprintf(stream,"%5d", A[i][j]);
        }
        fprintf(stream,"\n");
    }
    fprintf(stream,"\n");
    fclose(stream);
    return 0;
}

