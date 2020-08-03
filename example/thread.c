
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *print(void *tid);

int main(void)
{
    pthread_t thread[10];
    for (int i = 0; i < 10; i++)
    {
        int status = pthread_create(&thread[i], NULL, print, NULL);
        if (status != 0)
        {
            printf("error");
            exit(0);
        }
    }
    return 0;
}

void *print(void *tid)
{
    printf("hello,world\n");
    pthread_exit(0);
}