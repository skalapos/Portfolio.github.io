/*File: 
**  task.c
**  
**Name:
**  Steven Kalapos(0833056)
**
**Date Modified:
**  Spetember 30,2017
**
**Input:
**  task <grid-size> <#-iterations> -d
- 
- <grid-size> is one number that represents both the height and width of the grid. This is a required parameter.
- <#-iterations> is how many iterations of the game the program will run before exiting. This is a required parameter.
- -d This indicates that the program should print out the grid after each iteration. If the -d is present then the 
-    output should be displayed.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

struct Q
{
    int * array;
    int position;
    int end;
}queue;

//Global variables
int thread_count = 3;
int m; //size of baord
int iter;//number of iterations
int display;//1=display, 0=no display
int *game1,*game2;//1 will represent alive, 0 will represent not alive
struct Q *live, *dead;
pthread_mutex_t mutexL = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexD = PTHREAD_MUTEX_INITIALIZER;
int signalD = 0;

int currentBoard = 0; //0=game1; 1=game2

void initializeBoard();
void destroyBoard();
void printBoard();
void playSim(int start, int end);
void* taskP(void*threadRank);

int main(int argc, char const *argv[])
{
    long thread;
    pthread_t* thread_handles;

    //Initialize game parameters
    m = atoi(argv[1]);
    iter = atoi(argv[2]);
    if (argc >= 4)
        display = 1;
    else
        display = 0;


    initializeBoard();
    if (display == 1)
    {
        printf("Start Board\n");
        printBoard();
    }

    //Initialize queues
    live = malloc(sizeof(queue));
    dead = malloc(sizeof(queue));

    live->array = malloc(sizeof(int)*(m*m));
    dead->array = malloc(sizeof(int)*(m*m));

    live->position = 0;
    live->end = 0;

    dead->position = 0;
    dead->end = 0;

    thread_handles = malloc (thread_count*sizeof(pthread_t));

    for (int i = 0; i < iter; ++i)
    {
        //create threads
        for (thread = 0; thread < thread_count; thread++)
            pthread_create(&thread_handles[thread], NULL, taskP, (void*)thread);
    
        //join threads back
        for (thread = 0; thread < thread_count; thread++) 
            pthread_join(thread_handles[thread], NULL);

        currentBoard = (currentBoard+1)%2;
        signalD=0;
        
        live->position = 0;
        live->end = 0;

        dead->position = 0;
        dead->end = 0;

        if (display == 1)
        {
            printf("Iteration %d\n", i+1);
            printBoard();
        }
    }
    
    
    free(thread_handles);
    
    free(live->array);
    free(live);
    free(dead->array);
    free(dead);
    destroyBoard();

    return 0;
}

/*taskP
*
*Drives each thread to do specified task
*Arguments: threadRank - the number of the thread
*Return: NULL
*/
void* taskP(void *threadRank){
    unsigned int threadID = (uintptr_t) threadRank;

    if (threadID == 0) {
        int start = 0;
        int end = m*m-1;
        playSim(start,end);
        signalD = 1;

    }else if (threadID == 1) {
        while(signalD == 0 || live->position != live->end){
            int i;

            if (live->position == live->end)
                continue;
            pthread_mutex_lock(&mutexL);
            i = live->array[live->position];
            live->position++;
            pthread_mutex_unlock(&mutexL);
            if (currentBoard == 0)
                game2[i] = 1;
            else
                game1[i] = 1;
        }
    }else{
        while(signalD == 0 || dead->position != dead->end){
            int i;

            if (dead->position == dead->end)
                continue;
            pthread_mutex_lock(&mutexD);
            i = dead->array[dead->position];
            dead->position++;
            pthread_mutex_unlock(&mutexD);
            if (currentBoard == 0)
                game2[i] = 0;
            else
                game1[i] = 0;
        }
    }


    return NULL;
}

/*initializeBoard
*
*Initializes both game board matricies to play simulation
*Arguments: none
*Return: void
*/
void initializeBoard(){
    game1 = malloc(m*m*sizeof(int));    
    game2 = malloc(m*m*sizeof(int));

    //Initialize arrays with 0's
    for (int i = 0; i < m*m; ++i){
        game1[i] = 0;
        game2[i] = 0;
    }

    //games will start with no more than 33% of the board filled
    int startCells = (m*m)/3;
    //int cellCount = 0;
    srand((unsigned)time(NULL));
    int flip = 0;
    
    //choose spaces with living cells
    for (int i = 0; i < startCells; ++i)
    {
        flip = rand()%(m*m);
        
        if (game1[flip] == 1)
        {
            game1[flip] = 0;
            continue;
        }

        game1[flip] = 1; 
    }
    
    return;
}

/*destroyBoard
*
*Frees both game board matricies
*Arguments: none
*Return: void
*/
void destroyBoard(){
    free(game1);
    free(game2);
    return;
}
/*playSim
*
*Plays one round of the simulation
*Arguments: int start - start cell number; int end - end cell number
*Return: void
*/
void playSim(int start,int end){
    int cellNum;//used to determine what cell does

    for (int i = start; i <= end; ++i)
    {
        cellNum = 0;

        if (currentBoard==0)
        {
            if (i<m){//top row of numbers
                if (i == 0){
                    cellNum = cellNum+game1[i+1];
                    cellNum = cellNum+game1[i+m];
                    cellNum = cellNum+game1[i+m+1];
                }else if (i == m-1){
                    cellNum = cellNum+game1[i-1];
                    cellNum = cellNum+game1[i+m];
                    cellNum = cellNum+game1[i+m-1];
                }else{
                    cellNum = cellNum+game1[i+1];
                    cellNum = cellNum+game1[i-1];
                    cellNum = cellNum+game1[i+m];
                    cellNum = cellNum+game1[i+m+1];
                    cellNum = cellNum+game1[i+m-1];
                }

            }else if(i >= m*(m-1)){//bottom row numbers
                if (i == m*(m-1)){
                    cellNum = cellNum+game1[i+1];
                    cellNum = cellNum+game1[i-m];
                    cellNum = cellNum+game1[i-m+1];
                }else if (m*m -1){
                    cellNum = cellNum+game1[i-1];
                    cellNum = cellNum+game1[i-m];
                    cellNum = cellNum+game1[i-m-1];
                }else{
                    cellNum = cellNum+game1[i+1];
                    cellNum = cellNum+game1[i-1];
                    cellNum = cellNum+game1[i-m];
                    cellNum = cellNum+game1[i-m+1];
                    cellNum = cellNum+game1[i-m-1];
                }
            }else if(i%m == 0){//left side numbers
                cellNum = cellNum+game1[i+1];
                cellNum = cellNum+game1[i+m];
                cellNum = cellNum+game1[i+m+1];
                cellNum = cellNum+game1[i-m];
                cellNum = cellNum+game1[i-m+1];

            }else if (i%m == m-1){//right side numbers
                cellNum = cellNum+game1[i-1];
                cellNum = cellNum+game1[i+m];
                cellNum = cellNum+game1[i+m-1];
                cellNum = cellNum+game1[i-m];
                cellNum = cellNum+game1[i-m-1];
            }else{//everthing else
                cellNum = cellNum+game1[i+1];
                cellNum = cellNum+game1[i-1];
                cellNum = cellNum+game1[i+m];
                cellNum = cellNum+game1[i+m+1];
                cellNum = cellNum+game1[i+m-1];
                cellNum = cellNum+game1[i-m];
                cellNum = cellNum+game1[i-m+1];
                cellNum = cellNum+game1[i-m-1];
            }
            cellNum = cellNum+game1[i];

            if (cellNum==3){
                pthread_mutex_lock(&mutexL);
                live->array[live->end] = i;
                live->end = live->end+1;
                pthread_mutex_unlock(&mutexL);
            }
            else if(cellNum==4){
                if (game1[i]==0){
                 pthread_mutex_lock(&mutexD);  
                 dead->array[dead->end] = i;
                 dead->end = dead->end+1;
                 pthread_mutex_unlock(&mutexD); 
                }
            }
            else{
                pthread_mutex_lock(&mutexD);
                dead->array[dead->end] = i;
                dead->end = dead->end+1;
                pthread_mutex_unlock(&mutexD);
            }
        }else
        {
            if (i<m){//top row of numbers
                if (i == 0){
                    cellNum = cellNum+game2[i+1];
                    cellNum = cellNum+game2[i+m];
                    cellNum = cellNum+game2[i+m+1];
                }else if (i == m-1){
                    cellNum = cellNum+game2[i-1];
                    cellNum = cellNum+game2[i+m];
                    cellNum = cellNum+game2[i+m-1];
                }else{
                    cellNum = cellNum+game2[i+1];
                    cellNum = cellNum+game2[i-1];
                    cellNum = cellNum+game2[i+m];
                    cellNum = cellNum+game2[i+m+1];
                    cellNum = cellNum+game2[i+m-1];
                }

            }else if(i >= m*(m-1)){//bottom row numbers
                if (i == m*(m-1)){
                    cellNum = cellNum+game2[i+1];
                    cellNum = cellNum+game2[i-m];
                    cellNum = cellNum+game2[i-m+1];
                }else if (m*m -1){
                    cellNum = cellNum+game2[i-1];
                    cellNum = cellNum+game2[i-m];
                    cellNum = cellNum+game2[i-m-1];
                }else{
                    cellNum = cellNum+game2[i+1];
                    cellNum = cellNum+game2[i-1];
                    cellNum = cellNum+game2[i-m];
                    cellNum = cellNum+game2[i-m+1];
                    cellNum = cellNum+game2[i-m-1];
                }
            }else if(i%m == 0){//left side numbers
                cellNum = cellNum+game2[i+1];
                cellNum = cellNum+game2[i+m];
                cellNum = cellNum+game2[i+m+1];
                cellNum = cellNum+game2[i-m];
                cellNum = cellNum+game2[i-m+1];

            }else if (i%m == m-1){//right side numbers
                cellNum = cellNum+game2[i-1];
                cellNum = cellNum+game2[i+m];
                cellNum = cellNum+game2[i+m-1];
                cellNum = cellNum+game2[i-m];
                cellNum = cellNum+game2[i-m-1];
            }else{//everthing else
                cellNum = cellNum+game2[i+1];
                cellNum = cellNum+game2[i-1];
                cellNum = cellNum+game2[i+m];
                cellNum = cellNum+game2[i+m+1];
                cellNum = cellNum+game2[i+m-1];
                cellNum = cellNum+game2[i-m];
                cellNum = cellNum+game2[i-m+1];
                cellNum = cellNum+game2[i-m-1];
            }
            cellNum = cellNum+game2[i];

            if (cellNum==3){
                pthread_mutex_lock(&mutexL);
                live->array[live->end] = i;
                live->end = live->end+1;
                pthread_mutex_unlock(&mutexL);
            }
            else if(cellNum==4){
                if (game1[i]==0){
                 pthread_mutex_lock(&mutexD);  
                 dead->array[dead->end] = i;
                 dead->end = dead->end+1;
                 pthread_mutex_unlock(&mutexD); 
                }
            }
            else{
                pthread_mutex_lock(&mutexD);
                dead->array[dead->end] = i;
                dead->end = dead->end+1;
                pthread_mutex_unlock(&mutexD);
            }
        }   
    }
}

/*printBoard
*
*Prints out the current board to the screen
*Arguments: none
*Return: void
*/
void printBoard(){
    if (currentBoard == 0)
    {
        for (int i = 0; i < m*m; ++i){
            printf("%d", game1[i]);
            if ((i%m) == (m-1))
               printf("\n");
        }
        printf("\n");
    }else{
        for (int i = 0; i < m*m; ++i){
            printf("%d", game2[i]);
            if ((i%m) == (m-1))
               printf("\n");
        }
        printf("\n");
    }
}
