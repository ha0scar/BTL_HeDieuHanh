#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 50


void * philosopher(void * num);
void grab_chopstick(int, int, char * );
void down_chopsticks(int,int);
int food_on_table();

pthread_mutex_t chopstick[PHILOS];
pthread_t philo[PHILOS];
pthread_mutex_t food_lock;
int sleep_seconds = 0;
int flag[5] = {0, 0, 0, 0, 0} ;



int main(int argn, char ** argv) 
{
	FILE* fp = fopen("data1.1.csv", "a");
	fprintf(fp, "Time ,So Thuc An Con Lai\n");
	
    int i;
    //if (argn == 2)
    //    sleep_seconds = atoi(argv[1]);

    pthread_mutex_init( & food_lock, NULL);
    for (i = 0; i < PHILOS; i++)
        pthread_mutex_init( & chopstick[i], NULL);
    for (i = 0; i < PHILOS; i++) 
	{
        int * a = malloc(sizeof(int));
        * a = i;
        pthread_create( & philo[i], NULL, & philosopher, a);
    }

    for (i = 0; i < PHILOS; i++)
        pthread_join(philo[i], NULL);
  //      fprintf(fp,"%f,%d\n",(double) clock()/CLOCKS_PER_SEC,food_on_table());
    return food_on_table();
}

void *philosopher(void * num) {            // thread chay //
        int id;
        int i, left_chopstick, right_chopstick, f;

        id = *(int*) num;
        free(num);
        printf("Philosopher %d is done thinking and now ready to eat.\n", id);
        right_chopstick = id;
        left_chopstick = id + 1;

        /* Wrap around the chopsticks. */
        if (left_chopstick == PHILOS)
            left_chopstick = 0;
        printf("%d",f);
        while (f = food_on_table()) {

            //if (id == 1)
                //sleep(sleep_seconds);

            grab_chopstick(id, right_chopstick, "right ");
             flag[id] = 1; 
    	        if(  (flag[0] == 1)&&(flag[1] == 1)&& (flag[2] == 1)&& (flag[3] == 1)&&(flag[4] == 1)) // khi xay 
    	       {   
                      // printf ("Chuong Trinh dang bi treo, con lai %d FOOD",food_on_table());
                      // FILE* fp1 = fopen("data1.1.csv", "a");
                      // fprintf(fp1,"%f,%d\n"," ",food_on_table());
                    
                   down_chopsticks (left_chopstick, right_chopstick);
				}
             grab_chopstick(id, left_chopstick, "left");
			 flag[id] = 0;
            printf("Philosopher %d: eating.\n", id);
            usleep(DELAY * (FOOD - f + 1));  // thoi gian an us
            down_chopsticks(left_chopstick, right_chopstick);
        }
        

        printf("Philosopher %d is done eating.\n", id);   
        return (NULL);
    }

int food_on_table() 
{
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock( & food_lock); // tranh loi dong bo phat sinh
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock( & food_lock);
    return myfood;
}

void grab_chopstick(int phil,int c,char * hand) 
{
    pthread_mutex_lock( & chopstick[c]);
    printf("Philosopher %d: got %s chopstick %d\n", phil, hand, c);
}

void down_chopsticks(int c1,int c2)
{
    pthread_mutex_unlock(&chopstick[c1]);
    pthread_mutex_unlock(&chopstick[c2]);
}
