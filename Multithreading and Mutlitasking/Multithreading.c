#include <stdio.h>

#include <stdlib.h>

#include <sys/time.h>

#include <pthread.h>



long N[3] = {100000000, 1000000000, 10000000000};

int NUM_THREADS[3] =  {2, 4, 8};

int N_select;

int NTH_select;



/*



different threads add different numbers. For example, NUM_THREADS[NTH_select] = 2, when i=0, it adds all even numbers between 0 to n. When i=1, adds all odd numbers between 0 to n.



one thread adds even, another adds odd. 



*/

void* addup (long ind)

{

    long total = 0;

    for(long i = ind;i<=N[N_select]; i = i+NUM_THREADS[NTH_select]){

        total+=i;

    }

    pthread_exit((void*)total);//total's value is sum up below

}



int main(void)

{

 for(int y =0 ; y<3; y++)

 {

  N_select = y;

   for(int x = 0; x<3; x++)

   {

     NTH_select = x;

     long sum_up=0;

     unsigned long long sum=0;

     double dur = 0.0;

     int tn = NUM_THREADS[x];//number of threads at the time

     pthread_t thread_num[tn];

     double rtime, t;





      struct timeval start, end, res;

      gettimeofday(&start, NULL);

      for(int i = 0; i < NUM_THREADS[x]; i++)

            pthread_create(&thread_num[i], NULL, addup, (void*)i);//create the threads



      for(int i = 0; i < NUM_THREADS[x]; i++)

        {

            pthread_join(thread_num[i], (void*)&sum_up);//wait for thread to terminate, sum_up is what was added together in the thread

            sum += sum_up;

        }

      gettimeofday(&end, NULL);



      timersub(&end, &start, &res);

      t = (res.tv_sec * 1000000+ res.tv_usec);

      rtime = t/1000000;



      printf("This is the amount recieved by %d threads for a whole total of %ld: %llu\n", tn, N[y], sum);

      printf("This is the duration of %d threads for a whole total of %ld: %lf seconds\n", tn, N[y], rtime);

    }

  }

}
