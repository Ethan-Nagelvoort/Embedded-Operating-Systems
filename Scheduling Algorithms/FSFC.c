#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000
//#define WORKLOAD1 100000
//#define WORKLOAD2 100000
//#define WORKLOAD3 100000
//#define WORKLOAD4 100000
#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

void myfunction(int param){
	int i = 2;
	int j, k;
	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++){
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;}
			}	
		}
		i++;
	}
}

int main(int argc, char const *argv[])
{
        clock_t startOverall; 
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;
        clock_t p1, p2, p3, p4;
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
	
	//FCFS scheduling starts here
	startOverall = clock();
	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;
	if (running1 > 0)
    	{
        	kill(pid1, SIGCONT);
        	waitpid(pid1, &running1, 0);
        	p1 = clock();
    	}
        if (running2 > 0)
    	{
        	kill(pid2, SIGCONT);
        	waitpid(pid2, &running2, 0);
        	p2 = clock();
    	}
    	if (running3 > 0)
    	{
        	kill(pid3, SIGCONT);
        	waitpid(pid3, &running3, 0);
        	p3 = clock();
    	}
    	if (running4 > 0)
    	{
        	kill(pid4, SIGCONT);
        	waitpid(pid4, &running4, 0);
        	p4 = clock();
    	}
    	double r1 = (double)(p1-startOverall) / CLOCKS_PER_SEC;
    	double r2 = (double)(p2-startOverall) / CLOCKS_PER_SEC;
    	double r3 = (double)(p3-startOverall) / CLOCKS_PER_SEC;
    	double r4 = (double)(p4-startOverall) / CLOCKS_PER_SEC;
    	double avg = (r1 + r2 + r3 + r4) / 4;
    	printf("time response for process 1: %f seconds\n", r1);
    	printf("time response for process 2: %f seconds\n", r2);
    	printf("time response for process 3: %f seconds\n", r3);
    	printf("time response for process 4: %f seconds\n", r4);
    	printf("average time for all processes: %f seconds\n", avg);
}
