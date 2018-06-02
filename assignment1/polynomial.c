/*设多项式Pn(x)=anx^n+a(n-1)x^(n-1)+a(n-2)x^(n-2)+...+a1x1+a0
 *请给出计算Pn(x0)的方法并给出计算它的子程序
 * 如 a3 = 4, a2 = 5, a1 = 3, a0 = 1, x = 2
 */
#include <stdlib.h>
#include <stdio.h>
int polynomial(int n, int a[], int x)
{
    int tmp=1;
    int sum = a[0];
    for(int i = 1; i <= n ; i ++)
    {
        tmp = tmp * x;
        sum = sum + a[i] * tmp;    
    }
    return sum;
}
int polynomial2(int n, int a[], int x)
{
    int sum = 0;
    for(int i = 0; i <= n ; i ++)
    {
        sum = sum*x+a[i]; 
    }
    return sum;
}
int main(void)
{
    int n = 3;
    int a[] = {1, 1, 1, 1};
    int x = 2;
    int sum = polynomial(n, a, x);
    printf("sum = %d\n", sum);
    printf("sum = %d\n", polynomial2(n, a, x));
    return 0;
}

