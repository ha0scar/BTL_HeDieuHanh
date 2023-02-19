#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define PHILOS 7  // 5 triet gia
#define DELAY 5000  
#define FOOD 20  // thuc an 


void * philosopher(void * num);  // ham cua thread 
void grab_chopstick(int, int, char * );  // ham cam dua 1 chiec 
void down_chopsticks(int,int);  // ham bo dua xuong ( auto bo 2 chiec ) 
int food_on_table();         // thuc an tren ban ( giam thuc an di 1 )

pthread_mutex_t chopstick[PHILOS];     // tuong trung cho doi dua
pthread_t philo[PHILOS];            // khai bao thread  - 5 thread tuong ung voi 5 ng
pthread_mutex_t food_lock; // khoa mutex dam bao chi co 1 ng an tai 1 thoi diem
int sleep_seconds = 0;    // thoi gian ngu 
int flag[5] = {0, 0, 0, 0, 0} ;  //de phat hien deadlock  



int main(int argn, char ** argv) 
{
  	FILE* fp = fopen("data1.1.csv", "a");  // a ghi noi tiep trong file , w+ co the ghi de  
//	fprintf(fp, "Time ,So Thuc An Con Lai\n");
	
  	  int i;
    if (argn == 2)
       sleep_seconds = atoi(argv[1]);

    pthread_mutex_init( & food_lock, NULL);
    for (i = 0; i < PHILOS; i++) // tao ra 5 thread --
        pthread_mutex_init( & chopstick[i], NULL);
    for (i = 0; i < PHILOS; i++)  // truyen so nguyen vao thread * ep kieu  
	{
        int * a = malloc(sizeof(int));
        * a = i;
        pthread_create( & philo[i], NULL, & philosopher, a); // nhay vao void *philosopher(void * num) 
    }

    for (i = 0; i < PHILOS; i++)
        pthread_join(philo[i], NULL);
        fprintf(fp,"%f,%d\n",(double) clock()/CLOCKS_PER_SEC,food_on_table());  // lay thoi gian chay cua chuong trinh khi khong deadlock 
    return 0;
}

void *philosopher(void * num) { // ham tuong ung voi thread -
        int id; // dai dien cho so thu tu cua nguoi 
        int i, left_chopstick, right_chopstick, f;  // 

        id = *(int*) num;
        free(num); // cap phat dong nen dung xong phai giai phong
        printf("Philosopher %d is done thinking and now ready to eat.\n", id);
        right_chopstick = id;      
         left_chopstick = id + 1;  

        /* Wrap around the chopsticks. */
        if (left_chopstick == PHILOS) // 5 
            left_chopstick = 0;
        printf("%d",f);                       
        while (f = food_on_table()) {   // whie ( a=0 )  <=> while(0) voi a la 1 bien  

            //if (id == 1)
                //sleep(sleep_seconds);

            grab_chopstick(id, right_chopstick, "right ");
             flag[id] = 1; // co nay bat len 1 tuc la da cam duoc dua phai  v
    	        if(  (flag[0] == 1)&&(flag[1] == 1)&& (flag[2] == 1)&& (flag[3] == 1)&&(flag[4] == 1))  // phat hien ra deadlock 
    	       {   
                       printf ("Chuong Trinh dang bi treo, con lai %d FOOD",food_on_table());
	                       FILE* fp1 = fopen("data1.1.csv", "a");
	                       fprintf(fp1,"%f,%d\n"," ",food_on_table());
                      exit(0);
                    
                   //down_chopsticks (left_chopstick, right_chopstick);
				}
             grab_chopstick(id, left_chopstick, "left");  
			 flag[id] = 0;  // khi cam duoc du 2 chiec dua thi se tat ca
            printf("Philosopher %d: eating.\n", id);
            usleep(DELAY * (FOOD - f + 1));  // thoi gian an us random , thoi gian an cua moi thang la random 
            down_chopsticks(left_chopstick, right_chopstick);
        }
        

        printf("Philosopher %d is done eating.\n", id);   
        return (NULL);
    }

int food_on_table() 
{
    static int food = FOOD; // bien static la bien van ton tai sau ham ket thuc   
    int myfood;

    pthread_mutex_lock( & food_lock); // tranh loi dong bo phat sinh  // khoa san bong
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock( & food_lock);  // tra thia khoa 
    return myfood;
}

void grab_chopstick(int phil,int c,char * hand) //  c=1 cam dua 1 , c=2 cam dua 2  c=id doi dua
{
    pthread_mutex_lock( & chopstick[c]); // khoa chiec dua day lai 
    // khi 1 thang cam chiec dua co id =1 ma co 1 thang muon cam cai dua id =1 thi no se phai cho
    // 5 thang deu dang cho de cam dua trai  => xay ra deadlock
    printf("Philosopher %d: got %s chopstick %d\n", phil, hand, c);
}

void down_chopsticks(int c1,int c2)
{
    pthread_mutex_unlock(&chopstick[c1]);
    pthread_mutex_unlock(&chopstick[c2]);
}
