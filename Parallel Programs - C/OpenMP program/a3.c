/*File: 
**  a3.c
**  
**Name:
**  Steven Kalapos(0833056)
**
**Date Modified:
**  November 7,2017
**
**To run:
**  ./a3 <# of threads> <size of matrix> || ./a3 -g
-   if run with -g the program will test matricies of size 100, 1000, 10000 and 20000 with 1, 2 and 4 threads
-   else the users parameters will be used
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <omp.h>

//Global variables

float SIZE=0; //User created size
float TESTSIZE[4] = {100,1000,10000,20000}; //test sizes
int threadCount = 0; //user thread count
int testThreadC[3] = {1,2,4}; //test thread counts
int *matrix;
int *vector;
int *result;
int MAXNUM = 100;

void makeMatrix(float s);
void makeVector(float s);
void matrixMulti(int threads, float s);

int main(int argc, char const *argv[])
{
    time_t t;
    srand((unsigned)time(&t));

    if (argc > 2)
    {
        SIZE = atoi(argv[2]);
        threadCount = atoi(argv[1]);
        
        makeVector(SIZE);
        makeMatrix(SIZE);
        
        clock_t start, end;
        double cpuTime;
        start = clock();
        matrixMulti(threadCount,SIZE);
        end = clock();
        
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        
        printf("The program took %f seconds with %d threads\n",cpuTime,threadCount );
        
        free(vector);
        free(matrix);
        free(result);
    }else
    {
        printf("Size\tThreads\n");
        printf("\t1\t\t2\t\t4\n");

        for (int i = 0; i < 4; ++i)
        {
            printf("%.0f\t",TESTSIZE[i] );
            
            makeVector(TESTSIZE[i]);
            makeMatrix(TESTSIZE[i]);

            for (int j = 0; j < 3; ++j)
            {
                clock_t start = 0, end = 0;
                double cpuTime = 0;
                start = clock();
                matrixMulti(testThreadC[j], TESTSIZE[i]);
                end = clock();
                
                cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
                
                printf("%fs\t",cpuTime);
            }
            printf("\n");
            
            free(vector);
            free(matrix);
            free(result);
        }
    }
    return 0;
}

void makeVector(float s){
    
    int size = (int)s;
    vector = malloc(sizeof(int)*size);
    result = malloc(sizeof(int)*size);
    
    //initialize vector and result
    for (int i = 0; i < size; ++i)
    {
        vector[i]=rand()%MAXNUM;
        result[i]=0;
        //printf("%d ",vector[i] );
    }
    //printf("\n");

    return;
}

void makeMatrix(float s){
    
    int size = (int)s;
    matrix = malloc(sizeof(int)*(size*size));
    
    //initialize values
    for (int i = 0; i < size*size; ++i)
    {
        matrix[i]=rand()%MAXNUM;
        //printf("%d ",matrix[i] );
    }
    //printf("\n");

    return;
}

void matrixMulti(int threads, float s){
    
    int size = (int)s;
    //printf("%d ",threads );
    #pragma omp parallel num_threads(threads) 
    #pragma omp for schedule(auto) nowait
    for (int i = 0; i < size; ++i)
    {
        //printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
        float tempSum = 0;
        for (int j = 0; j < size; ++j)
        {
            int x = (i*size)+j; //position in matrix
            tempSum = tempSum+(vector[j]*matrix[x]);
        }
        result[i] = tempSum;
    }
}
