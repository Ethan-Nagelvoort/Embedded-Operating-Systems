#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>


#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

//#define WORKLOAD1 100000
//#define WORKLOAD2 100000
//#define WORKLOAD3 100000
//#define WORKLOAD4 100000

#define QUANTUM1 6800
#define QUANTUM2 6800
#define QUANTUM3 6800
#define QUANTUM4 6800


void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}

int main(int argc, char const *argv[])
{
        struct timeval start, res;
        struct timeval end[4];
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;

	pid1 = fork();

	if (pid1 == 0){

		myfunction(WORKLOAD1);

		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();

	if (pid2 == 0){

		myfunction(WORKLOAD2);

		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();

	if (pid3 == 0){

		myfunction(WORKLOAD3);

		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();

	if (pid4 == 0){

		myfunction(WORKLOAD4);

		exit(0);
	}
	kill(pid4, SIGSTOP);
        //RR scheduling starts here
	gettimeofday(&start, NULL);
	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0){
		
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
			gettimeofday(&end[0], NULL);
		}
		if (running2 > 0){
			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
			gettimeofday(&end[1], NULL);
		}
		if (running3 > 0){
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
			gettimeofday(&end[2], NULL);
		}
		if (running4 > 0){
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
			gettimeofday(&end[3], NULL);
		}
		  waitpid(pid1, &running1, WNOHANG);
		  waitpid(pid2, &running2, WNOHANG);
		  waitpid(pid3, &running3, WNOHANG);
		  waitpid(pid4, &running4, WNOHANG);
	}
	double t[4] = {0,0,0,0};
	for(int i=0; i<4; i++)
	{
          timersub(&end[i], &start, &res);
          t[i] = (res.tv_sec * 1000000 + res.tv_usec);
          t[i] = t[i]/1000000;
	}
    	double avg = (t[0] + t[1] + t[2] + t[3]) / 4;
    	/*printf("Process 1 ID: %d\n", pid1);
    	printf("Process 2 ID: %d\n", pid2);
    	printf("Process 3 ID: %d\n", pid3);
    	printf("Process 4 ID: %d\n", pid4);
    	printf("time response for process 1: %f seconds\n", t[0]);
    	printf("time response for process 2: %f seconds\n", t[1]);
    	printf("time response for process 3: %f seconds\n", t[2]);
    	printf("time response for process 4: %f seconds\n", t[3]);*/
    	printf("average time for all processes: %f seconds.\n",avg);
}
