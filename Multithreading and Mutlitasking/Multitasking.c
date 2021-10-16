#include <stdlib.h>

#include <stdio.h>

#include <time.h>

#include <sys/wait.h>

#include <unistd.h>



unsigned long long sum_final = 0;



unsigned long long twoTasks(long N_value)

{

	long accu = N_value/2;

	int fd[2];

	unsigned long long sum, sum1, sum2 = 0;

	pipe(fd);//create pipe for two tasks

	pid_t pid = fork();//split process into two task: parent and child

	if(pid==0)

	{

		close(fd[0]);//close this part of the pipe for child

		for(long i=0; i<accu; i++)

		{

			sum1 = sum1 + i;

		}

		write(fd[1], &sum1, sizeof(sum1));//have child write to pipe to put sum1 into pipe

		exit(0);

	}

	else if(pid>0)

	{

	        close(fd[1]);//close this part of the pipe for parent

		wait(NULL);//wait till child is terminated

		for(long i=accu; i<=N_value; i++)

		{

			sum2 = sum2 + i;

		}

		read(fd[0], &sum1, sizeof(sum1));//have parent read to pipe to get sum1

		sum = sum1 + sum2;

	}

	return sum;

}





unsigned long long fourTasks(long N)

{

    unsigned long long sumP = 0, sumC0 = 0, sumC1 = 0, overall_sum = 0, i;

    unsigned long long overall_sum1 = 0, overall_sum2 = 5;

    pid_t pid;

    int fd1[2];

    int fd2[2]; 

    pipe(fd1);

    pipe(fd2);

            

    if (fork() == 0)

    {

        if((pid = fork()) == 0)

        {

            for(i = 0; i <= N; i = i + 4)

                sumC0  = sumC0 + i;

            close(fd2[0]); 

            write(fd2[1], &sumC0, sizeof(sumC0));

            exit(0);

        }

        else

        {

            wait(NULL);

            close(fd2[1]); 

            read(fd2[0], &overall_sum1, sizeof(overall_sum1));

            for(i = 1; i <= N; i = i + 4)

                sumC0  = sumC0 + i;

            overall_sum1 = overall_sum1 + sumC0;

        }

        for(i = 2; i <= N; i = i + 4)

            sumC1  = sumC1 + i;

        overall_sum2 = overall_sum1 + sumC1;

        close(fd1[0]); 

        write(fd1[1], &overall_sum2, sizeof(overall_sum2));

        exit(0);

    }

    else

    {

        wait(NULL);

        close(fd1[1]); 

        read(fd1[0], &overall_sum, sizeof(overall_sum));

        for(i = 3; i <= N; i = i + 4)

            sumP  = sumP + i;

        overall_sum = overall_sum + sumP;

       return overall_sum;

    }

    

}



long eightTaskSum(long start, long N_value){

    long sum = 0;

    for(long i = start; i<=N_value; i = i + 8)

        sum += i;

    return sum;

}



unsigned long long eightTasks(long N_value)

{

    unsigned long long sum = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0;

    unsigned long long total = 0, total1 = 0, total2 = 0, total3 = 0, total4 = 0, total5 = 0, total6 = 0;

    pid_t pid, pid1, pid2;

    int fd[16];

    int c = 0;

    for (int i = 0; i < 8; i++) {

        pipe(&fd[2*i]);

    }

    pid = fork();

    pid1 = fork();

    pid2 = fork();

    int t = 0;

    if(pid == 0 && pid1 == 0 && pid2 == 0)//t8

    {

     t = 8;

    }

    else if (pid == 0 && pid1 == 0 && pid2 > 0) // t4

    {

        t= 4;

    }

    else if (pid == 0 && pid1 > 0 && pid2 == 0) // t7

    {

        t=7;

    }

    else if (pid == 0 && pid1 > 0 && pid2 > 0) //  t2

    {

       t=2;

        

    }

    else if (pid > 0 && pid1 == 0 && pid2 == 0) //  t6

    {

        t=6;

    }

    else if (pid > 0 && pid1 == 0 && pid2 > 0) //  t3

    {

        t=3;

    }

    else if (pid > 0 && pid1 > 0 && pid2 == 0) //  t5

    {

        t=5;

    }

    else if(pid>0 && pid>0 && pid>0)//t1

    {

      t=1;

    }

     

    switch(t)

    {

	    case 8: // t8

	    { 

		close(fd[0]);

		sum  = eightTaskSum(0, N_value);

		write(fd[1], &sum, sizeof(sum));

		exit(0);

	    }

	    case 4: // t4

	    {

		wait(NULL);

		close(fd[2]);

		close(fd[1]);

		read(fd[0], &total, sizeof(total));

		sum1  = eightTaskSum(1, N_value);

		total += sum1;

		write(fd[3], &total, sizeof(total));

		exit(0);

	    }

	    case 7: // t7

	    {

		close(fd[4]);

		sum3  = eightTaskSum(2, N_value);

		write(fd[5], &sum3, sizeof(sum3));

		exit(0);

	    }

	    case 2://  t2

	    {

		wait(NULL);

		close(fd[5]);

		close(fd[6]);

		close(fd[3]);

		read(fd[2], &total1, sizeof(total1));

		read(fd[4], &total2, sizeof(total2));

		sum3  = eightTaskSum(3, N_value);

		total2 = total2 + total1 + sum3;

		write(fd[7], &total2, sizeof(total2));

		exit(0);

		

	    }

	    case 6: //  t6

	    {

		close(fd[8]);

		sum4  = eightTaskSum(4, N_value);

		write(fd[9], &sum4, sizeof(sum4));

		exit(0);

	    }

	    case 3: //  t3

	    {

		wait(NULL);

		close(fd[10]);

		close(fd[9]);

		read(fd[8], &total3, sizeof(total3));

		sum5  = eightTaskSum(5, N_value);

		total3 += sum5;

		write(fd[11], &total3, sizeof(total3));

		exit(0);

	    }

	    case 5: //  t5

	    {

		sum6  = eightTaskSum(6, N_value);

		close(fd[12]);

		write(fd[13], &sum6, sizeof(sum6));

		exit(0);

	    }

	    case 1: //t1

	    {

		wait(NULL);

		close(fd[7]);

		close(fd[11]);

		close(fd[13]);

		read(fd[6], &total4, sizeof(total4));

		read(fd[10], &total5, sizeof(total5));

		read(fd[12], &total6, sizeof(total6));

		sum7  = eightTaskSum(7,N_value);

		total6 = total6 + total5 + total4 + sum7;

	    }

    }

    return total6;

    }







int main(void)

{

unsigned long long sum = 0;

 //pick an N value

 //long n = 100000000;

 //long n = 1000000000;

 long n = 10000000000;

 //int taskNum = 2;

 int taskNum =4;

 //int taskNum =8;

 //sum = twoTasks(n);

 sum = fourTasks(n);

 //sum = eightTasks(n);

 printf("%d tasks, N=%ld, total = %llu\n",taskNum, n, sum_final);

}
