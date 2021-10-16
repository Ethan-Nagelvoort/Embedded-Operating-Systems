#include <stdlib.h>

#include <sys/time.h>

#include <stdio.h>



long N[3] = {100000000,1000000000,10000000000};



int main()

{

    unsigned long long total = 0;

    



    for(int x = 0; x < 3; x++){

    double rtime;

    double t;

    struct timeval start, end, res;



        gettimeofday(&start,NULL);

        for(long i=0; i <= N[x]; i++){

                    total += i;

            }

            gettimeofday(&end, NULL);

            timersub(&end,&start, &res);

            t=(res.tv_sec * 1000000 + res.tv_usec);

            rtime = t/1000000;

            printf("The Sum from 0 to %ld is: %llu \n", N[x], total);

            printf("The Duration from 0 to %ld is: %lf\n", N[x], rtime);

            total = 0;

    }

}
