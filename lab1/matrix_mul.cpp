#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string.h>
using   namespace   std;

int main()
{
    clock_t start, finish;
    clock_t start1, finish1;

    int i,j,k;
    //初始化两个1000*1000的矩阵
    int (*a)[1000],(*b)[1000];
    a = new int[1000][1000];
    b = new int[1000][1000];


    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 1000; j++)
        {
            a[i][j] = i % (j+1);
            b[i][j] = i / (j+1);
        }
    }
    //存放A*B的结果
    int (*c)[1000],(*d)[1000];
    c = new int[1000][1000];
    d = new int[1000][1000];

    //初始化为0
    memset(c,0, 1000*1000*sizeof(int));
    memset(d,0, 1000*1000*sizeof(int));

    start = clock();
    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 1000; j++)
        {
            for (k = 0; k < 1000; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }

        }
    }
    finish = clock();

    start1 = clock();

    //可以修改的部分  开始
    //======================================================
    for(i = 0; i < 1000; i += 4) {
        for(k = 0; k < 1000; ++k) {
            int r0 = a[i][k];
            int r1 = a[i + 1][k];
            int r2 = a[i + 2][k];
            int r3 = a[i + 3][k];
            for (j = 0; j < 1000; ++j) {
                d[i][j] += r0 * b[k][j];
                d[i + 1][j] += r1 * b[k][j];
                d[i + 2][j] += r2 * b[k][j];
                d[i + 3][j] += r3 * b[k][j];
            }
        }
    }
    //可以修改的部分   结束
    //======================================================
    finish1 = clock();



    //对比两次的结果
    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 1000; j++)
        {
            if (c[i][j] != d[i][j])
            {
                cout<<"you have got an error in algorithm modification!"<<endl;
                exit(1);
            }

        }
    }



    cout<<"time spent for original method : "<<finish - start<<" ms"<<endl;
    cout<<"time spent for new method : "<<finish1 - start1<<" ms"<<endl;
    return 0;
}
