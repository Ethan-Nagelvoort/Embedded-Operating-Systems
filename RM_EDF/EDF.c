#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>


struct textTable { // Reads the first row of the Text File table
    int nTasks; // <# of tasks>
    int exec_time; // <time the system will execute up to in seconds>
    int cpu1188; // <Active CPU Power @ 1188 MHz>
    int cpu918; // <Active CPU Power @ 918 MHz>
    int cpu648; // <Active CPU Power @ 648 MHz>
    int cpu384; // <Active CPU Power @ 384 MHz>
    int idle;
};

struct Task { //All other rows represent
    char name[2]; // <Name of task>
    int deadline; // <deadline/period
    int WCET1; // WCET @ 1188 MHz
    int WCET2; // WCET @ 918 MHz
    int WCET3; // WCET @ 648 MHz
    int WCET4; // WCET @ 384 MHz
};

struct rQueue {
    int arrival; // arrival Time 
    int timeleft; // How much each task has left 
    struct Task myTask;
};

int main(int argc, char* argv[])
{
    FILE* textFile;
    int algo;

    struct textTable table;
    struct Task Tasks[5];
    struct Task myTasks[5];

    //***********
    

    //***********

    //**************File Scanning************** 
    textFile = fopen(argv[1], "r"); // opening the file for reading purposes

    if (!textFile)
    {
        printf("File not found\n");
        return 1;
    }    
    
    fscanf(textFile, "%d %d %d %d %d %d %d", &table.nTasks, &table.exec_time, &table.cpu1188, &table.cpu918, &table.cpu648, &table.cpu384, &table.idle); // Stores values of first line

    for (int i = 0; i < 5; i++)
    {
        fscanf(textFile, "%s %d %d %d %d %d", Tasks[i].name, &Tasks[i].deadline, &Tasks[i].WCET1, &Tasks[i].WCET2, &Tasks[i].WCET3, &Tasks[i].WCET4); // Reads and stores value into Output Table
        myTasks[i] = Tasks[i];
    }

    //*****************************************
    //************Algorithm checker**********
    algo = (strcmp(argv[2], "EDF") == 0) ? 1 : 2 ; // If the second argument is EDF, return true, else 0

    if(algo == 1)
    {
        struct rQueue rTask[table.nTasks];
    
        int current = 0;
        int prev = 0;
        int execCount = 0;
        int iTime = 1;
        int idleTime = 0;

        double energy = 0.0, totEn = 0.0, t, rtime;
        double x;

        float idperc; //idle percentage
        struct timeval start, end, res;
        
        printf("Earliest Deadline First Scheduling: \n");
        //run_edf(myTasks, table, table.nTasks);
        //*****EDF
        for (int i =0; i < table.nTasks; i++){ // Utilization <= 1?
            x += ((double)myTasks[i].WCET1 / (double)myTasks[i].deadline);
        }

        if (x <= 1) 
        {
            int i = 0;
            while(i != table.nTasks){ // Sets the task's initial start, stores tasks, and gets worst case execution-time
                rTask[i].arrival = 1; 
                rTask[i].myTask = myTasks[i];
                rTask[i].timeleft = myTasks[i].WCET1;
                i++;
            }

            gettimeofday(&start, NULL);
            for (int secs = 1; secs <= table.exec_time; secs++) // Starts counting up to the execution time of 1000
            {
                for (int i = 0; i <= table.nTasks - 1; i++)
                {
                    for (int j = 0; j < table.nTasks - 1; j++)
                    {
                        if (rTask[j+1].myTask.deadline < rTask[j].myTask.deadline)
                        {
                            struct rQueue temp = rTask[j];
                            rTask[j] = rTask[j+1];
                            rTask[j+1] = temp;
                            //qChange(rTask,j,j+1); //Grabs the task which obtains earliest deadline
                        }
                    }
                }

                current = 0;

                if (rTask[current].arrival > secs) // if arrival time is greater than current execution time
                {
                    for (int i = 0; i < table.nTasks; i++)
                    {
                        if (rTask[i].arrival <= secs) // if current arrival time is less than current execution time, should idle
                        {
                            current = i;
                            break; // breaks through and creates a idle stage
                        }
                        current = -1;
                    }

                    if (current != -1 && rTask[current].timeleft == 0) // if current arrival is less than execution time and the task does not have any more exec time left, calculate start/finish, and energy used
                    {
                        energy = (rTask[current].myTask.WCET1 * table.cpu1188) / 1000;
                        printf("%d %s %d %d %0.2lfJ\n", iTime, rTask[current].myTask.name, 1188, execCount, energy);
                        rTask[current].arrival = rTask[current].myTask.deadline;
                        rTask[current].myTask.deadline += rTask[current].myTask.deadline;
                        execCount = 0;
                        iTime = secs;
                        totEn += energy;
                    }
                    else //continues counting and reduces remaining time if its not done executing
                    {
                        execCount++; rTask[current].timeleft--;
                    }
                }
                else if (rTask[current].arrival < secs)
                {
                    if (rTask[current].timeleft == 0) // task no longer has execution time
                    {
                        energy = (rTask[current].myTask.WCET1 * table.cpu1188) / 1000;
                        printf("%d %s %d %d %0.2lfJ\n", iTime, rTask[current].myTask.name, 1188, execCount, energy);
                        rTask[current].arrival = rTask[current].myTask.deadline;
                        rTask[current].myTask.deadline += rTask[current].myTask.deadline;
                        rTask[current].timeleft = rTask[current].myTask.WCET1;
                        execCount = 0;
                        iTime = secs;
                        totEn += energy;
                    }
                    else 
                    {
                        execCount++; rTask[current].timeleft--;
                    }
                }

                if (current == -1)
                {
                    idleTime++;
                }
                else if (prev == -1 && current > -1)
                {
                    energy = (idleTime * table.idle) / 1000;
                    printf("%d %s %s %d %0.2lfJ\n", iTime, "IDLE", "IDLE", execCount, energy);
                    iTime = secs;
                    execCount = 0;
                    totEn += energy;
                }

                idperc = ((float)idleTime/1000) * 100;
                prev = current;
                gettimeofday(&end, NULL);
                timersub(&end, &start, &res);
            }
                t = (res.tv_sec * 1000000 + res.tv_usec);
                rtime = t/1000000;
                printf("Total Energy: %lf J\nIdle Time: %lf %%\nDuration: %lf secs\n", totEn , idperc, rtime);
        }
        else
        {
            printf("EDF Not Found\n");
        }
        //******END OF EDF
    }
    //***************************************
    return 0;
}
