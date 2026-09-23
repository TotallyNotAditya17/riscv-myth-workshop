// ============================================================================
// VSDIAT RISC-V MYTH Workshop - Lab 1: C Program to Sum 1 to N
// Author: Aditya Nanda (TotallyNotAditya17)
// ============================================================================

#include<stdio.h>
int main()
{
    int i,sum=0,n=9;
    for(i=1;i<=n;i++)
    {
        sum+=i;
    }
    printf("The sum of numbers from 1 to %d is %d\n", n, sum);
    return 0;
}
