#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h> 
#include <sys/time.h>

struct row1 {//info from row 1 of input file
    int TaskNum;
    int execution;
    int cpu1188;
    int cpu918;
    int cpu648;
    int cpu384;
    int idle;
};

struct Task {
    char taskName[3];
    int deadline;
    int WCET1;
    int WCET2;
    int WCET3;
    int WCET4;
    int status;
    int execu;
    int power;
    int cpuFreq;
};


int RM_remainder(int time, int ex, int dead, int i, struct Task Tasks[]);
int RM_exec(int ex, struct Task Tasks[], int i);
void RM_status(int t);
int RM_deadline(int time, int dead, struct Task Tasks[], int i);
int nextTask(int flag, int ex0, int ex1, int ex2, int ex3, int ex4);

int main(int argc, char *argv[])
{
   int idleT = 0;
   double energyC = 0;
   struct timeval start, res;
   struct timeval end;
   FILE *ptr_file;
    int rm=0;
    int edf=0;
    int ee=0;
    struct row1 CPU;
    struct Task Tasks[5];
    ptr_file =fopen(argv[1],"r");
    
    if(ptr_file == 0)
    {
      printf("File is not here");
      return 1;
    }
    
    if (strcmp(argv[2],"EDF") == 0)
    {
        printf("We are using EDF scheduling algorithm\n");
        edf = 1;
    }
    else if (strcmp(argv[2],"RM") == 0)
    {
        printf("We are using RM scheduling algorithm\n");
        rm = 1;
    }
    else
    {
        printf("Scheduling algorithm is not specified\n");
    }
    
    if(argv[3] != NULL && strcmp(argv[3],"EE")==0)
    {
        printf("We are using the energy efficient version of this schedule\n");
        ee=1;
    }
    
    fscanf(ptr_file,"%d %d %d %d %d %d %d", &CPU.TaskNum, &CPU.execution, &CPU.cpu1188, &CPU.cpu918, &CPU.cpu648, &CPU.cpu384, &CPU.idle);
    fscanf(ptr_file,"%s %d %d %d %d %d", Tasks[0].taskName, &Tasks[0].deadline, &Tasks[0].WCET1, &Tasks[0].WCET2, &Tasks[0].WCET3, &Tasks[0].WCET4);
    fscanf(ptr_file,"%s %d %d %d %d %d", Tasks[1].taskName, &Tasks[1].deadline, &Tasks[1].WCET1, &Tasks[1].WCET2, &Tasks[1].WCET3, &Tasks[1].WCET4);
    fscanf(ptr_file,"%s %d %d %d %d %d", Tasks[2].taskName, &Tasks[2].deadline, &Tasks[2].WCET1, &Tasks[2].WCET2, &Tasks[2].WCET3, &Tasks[2].WCET4);
    fscanf(ptr_file,"%s %d %d %d %d %d", Tasks[3].taskName, &Tasks[3].deadline, &Tasks[3].WCET1, &Tasks[3].WCET2, &Tasks[3].WCET3, &Tasks[3].WCET4);
    fscanf(ptr_file,"%s %d %d %d %d %d", Tasks[4].taskName, &Tasks[4].deadline, &Tasks[4].WCET1, &Tasks[4].WCET2, &Tasks[4].WCET3, &Tasks[4].WCET4);
    if(ee == 1)
    {
     int freq[4] = {1188, 918, 648, 384};
     int pp[4] = {CPU.cpu1188, CPU.cpu918, CPU.cpu648, CPU.cpu384};
     double ff = pow(2, .2);
     ff = 5*(ff-1);
     //printf("ff = %lf\n", ff);
     double t1, t2, t3, t4, t5, t6, tTot = 0;
     double old_tTot = 0;
     int exe1[4] = {Tasks[0].WCET1, Tasks[0].WCET2, Tasks[0].WCET3, Tasks[0].WCET4};
     int exe2[4] = {Tasks[1].WCET1, Tasks[1].WCET2, Tasks[1].WCET3, Tasks[1].WCET4};
     int exe3[4] = {Tasks[2].WCET1, Tasks[2].WCET2, Tasks[2].WCET3, Tasks[2].WCET4};
     int exe4[4] = {Tasks[3].WCET1, Tasks[3].WCET2, Tasks[3].WCET3, Tasks[3].WCET4};
     int exe5[4] = {Tasks[4].WCET1, Tasks[4].WCET2, Tasks[4].WCET3, Tasks[4].WCET4};
     for(int i = 0; i<4; i++)
     { 
       t1 = ((double)exe1[i]/Tasks[0].deadline);
       for(int ii=0; ii<4; ii++)
       {
        t2 = ((double)exe2[ii]/Tasks[1].deadline);
        for(int iii=0; iii<4; iii++)
        {
         t3 = ((double)exe3[iii]/Tasks[2].deadline);
         for(int iiii=0; iiii<4; iiii++)
         {
           t4 = ((double)exe4[iiii]/Tasks[3].deadline);
           for(int g=0; g<4; g++)
           {
             t5 = ((double)exe5[g]/Tasks[4].deadline);
             tTot = t1+t2+t3+t4+t5;
             //printf("tTot = %lf\n", tTot);
             if((tTot<ff) && (tTot>old_tTot))
             {
               old_tTot = tTot;
               Tasks[0].execu = exe1[i];
               Tasks[1].execu = exe2[ii];
               Tasks[2].execu = exe3[iii];
               Tasks[3].execu = exe4[iiii];
               Tasks[4].execu = exe5[g];
               Tasks[0].power = pp[i];
               Tasks[1].power = pp[ii];
               Tasks[2].power = pp[iii];
               Tasks[3].power = pp[iiii];
               Tasks[4].power = pp[g];
               Tasks[0].cpuFreq = freq[i];
               Tasks[1].cpuFreq = freq[ii];
               Tasks[2].cpuFreq = freq[iii];
               Tasks[3].cpuFreq = freq[iiii];
               Tasks[4].cpuFreq = freq[g];
             }
           }
         }
        }
       }
     }
     if(Tasks[0].execu==0)
     {
       printf("There was no efficient energy RM\n");
       return 0;
     }
    }
    else
    {
      for(int i=0; i<5; i++)
      {
        Tasks[i].execu = Tasks[i].WCET1;
        Tasks[i].power = CPU.cpu1188;
        Tasks[i].cpuFreq = 1188;
      }
    }
    if(rm == 1)
    {
    
    for (int i = 0; i < 5; i++)
    {
         Tasks[i].status = 0;
    }
    if(ee==0){
    double sum = 0;
    for(int i=0; i<5; i++)
    {
      sum = sum+ (((double)Tasks[i].execu)/(Tasks[i].deadline));
    }
    //printf("sum = %lf", sum);
    double feas = pow(2, .2);
    feas = 5*(feas-1);
    //printf("feas = %lf", feas);
    if(sum>feas)
    {
      printf("The RM inequality does not work with this RM schedule\n");
    }
    }
    //setting priority
    gettimeofday(&start, NULL);
    for(int j=0; j<5; j++)
    {
      int x = Tasks[j].deadline;
    for(int i=0; i<5; i++)
    {
      if(x<Tasks[i].deadline)
      {
        struct Task t = Tasks[j];
        Tasks[j] = Tasks[i];
        Tasks[i] = t;
      }
    }
    }
    int time = 1;
    int back, back1, back2, back3, back4 = 0;
    int t = 0;
    int flag = 1;
    int work0, work1, work2, work3, work4 = 0;
    int ex0 = Tasks[0].execu;
    int ex1 = Tasks[1].execu;
    int ex2 = Tasks[2].execu;
    int ex3 = Tasks[3].execu;
    int ex4 = Tasks[4].execu;
    int dead0 = Tasks[0].deadline;
    int dead1 = Tasks[1].deadline;
    int dead2 = Tasks[2].deadline;
    int dead3 = Tasks[3].deadline;
    int dead4 = Tasks[4].deadline;
    int a =1 ,b = 1,c = 1,d = 1,e = 1;
    int idleArr[5] = {0,0,0,0,0};
     while(time<CPU.execution)
     {
       switch(flag)
       {
       case 1:
       {
         t = ex0;
         double e = (Tasks[0].power*pow(10,-3))*t;
         energyC = energyC + e;
         if(t != 0)
         {
         printf("%d %s %d %d %lfJ\n", time, &Tasks[0].taskName, Tasks[0].cpuFreq, t, e);
         }
         time = time + t;
         dead0 = RM_deadline(time, dead0, Tasks, 0);
         dead1 = RM_deadline(time, dead1, Tasks, 1);
         dead2 = RM_deadline(time, dead2, Tasks, 2);
         dead3 = RM_deadline(time, dead3, Tasks, 3);
         dead4 = RM_deadline(time, dead4, Tasks, 4);
         ex0 = RM_exec(ex0, Tasks, 0);
         ex1 = RM_exec(ex1, Tasks, 1);
         ex2 = RM_exec(ex2, Tasks, 2);
         ex3 = RM_exec(ex3, Tasks, 3);
         ex4 = RM_exec(ex4, Tasks, 4);
         if(time >= CPU.execution)
         {
         break;
         }
        ex0 = 0;
        t=0;
        flag = nextTask(flag, ex0, ex1, ex2, ex3, ex4);
        break;
       }
       
       case 2:
       {
         if(time+ex1 > dead0)
         {
           t = RM_remainder(time, ex1, dead0, 0, Tasks);
         }
         else
         {
           t = ex1;
         }
         double e = (Tasks[1].power*pow(10,-3))*t;
         energyC = energyC + e;
         if(t != 0)
         {
          printf("%d %s %d %d %lfJ\n", time, &Tasks[1].taskName, Tasks[1].cpuFreq, t, e);
         }
         time = t + time;
         dead0 = RM_deadline(time, dead0, Tasks, 0);
         dead1 = RM_deadline(time, dead1, Tasks, 1);
         dead2 = RM_deadline(time, dead2, Tasks, 2);
         dead3 = RM_deadline(time, dead3, Tasks, 3);
         dead4 = RM_deadline(time, dead4, Tasks, 4);
         ex0 = RM_exec(ex0, Tasks, 0);
         ex1 = RM_exec(ex1, Tasks, 1);
         ex2 = RM_exec(ex2, Tasks, 2);
         ex3 = RM_exec(ex3, Tasks, 3);
         ex4 = RM_exec(ex4, Tasks, 4);
         if(time >= CPU.execution)
         {
         break;
         }
         ex1 = ex1 - t;
         t=0;
         flag = nextTask(flag, ex0, ex1, ex2, ex3, ex4);
         break;
       }
       
       case 3:
       {
         if(time+ex2 > dead0)
         {
            t = RM_remainder(time, ex2, dead0, 0, Tasks);
         }
         else if(time+ex2 > dead1)
         {
            t = RM_remainder(time, ex2, dead1, 1, Tasks);
         }
         else 
         {
           t = ex2;
         }
         double e = (Tasks[2].power*pow(10,-3))*t;
         energyC = energyC + e;
         if(t != 0)
         {
         printf("%d %s %d %d %lfJ\n", time, &Tasks[2].taskName, Tasks[2].cpuFreq, t, e);
         }
         time = t + time;
         dead0 = RM_deadline(time, dead0, Tasks, 0);
         dead1 = RM_deadline(time, dead1, Tasks, 1);
         dead2 = RM_deadline(time, dead2, Tasks, 2);
         dead3 = RM_deadline(time, dead3, Tasks, 3);
         dead4 = RM_deadline(time, dead4, Tasks, 4);
         ex0 = RM_exec(ex0, Tasks, 0);
         ex1 = RM_exec(ex1, Tasks, 1);
         ex2 = RM_exec(ex2, Tasks, 2);
         ex3 = RM_exec(ex3, Tasks, 3);
         ex4 = RM_exec(ex4, Tasks, 4);
         if(time >= CPU.execution)
         {
         break;
         }
         ex2 = ex2 - t;
         t=0;
         flag = nextTask(flag, ex0, ex1, ex2, ex3, ex4);
         break;
       }
       
       case 4:
       {
         if(time+ex3 > dead0)
         {
            //printf("inside ex0>0\n");
            //printf("dead0 = %d, ex3 = %d, time = %d\n", dead0, ex3, time);
            t = RM_remainder(time, ex3, dead0, 0, Tasks);
         }
         else if(time+ex3 > dead1)
         {
            t = RM_remainder(time, ex3, dead1, 1, Tasks);
         }
         else if(time+ex3 > dead2)
         {
            t = RM_remainder(time, ex3, dead2, 2, Tasks);
         }
         else 
         {
           t = ex3;
         }
         double e = (Tasks[3].power*pow(10,-3))*t;
         energyC = energyC + e;
         if(t != 0)
         {
         printf("%d %s %d %d %lfJ\n", time, &Tasks[3].taskName, Tasks[3].cpuFreq, t, e);
         }
         time = t + time;
         dead0 = RM_deadline(time, dead0, Tasks, 0);
         dead1 = RM_deadline(time, dead1, Tasks, 1);
         dead2 = RM_deadline(time, dead2, Tasks, 2);
         dead3 = RM_deadline(time, dead3, Tasks, 3);
         dead4 = RM_deadline(time, dead4, Tasks, 4);
         ex0 = RM_exec(ex0, Tasks, 0);
         ex1 = RM_exec(ex1, Tasks, 1);
         ex2 = RM_exec(ex2, Tasks, 2);
         ex3 = RM_exec(ex3, Tasks, 3);
         ex4 = RM_exec(ex4, Tasks, 4);
         if(time >= CPU.execution)
         {
         break;
         }
         ex3 = ex3 - t;
         t=0;
         flag = nextTask(flag, ex0, ex1, ex2, ex3, ex4);
         break;
       }
       
       case 5:
       {
         //printf("ex- %d %d %d %d %d\n", ex0, ex1, ex2, ex3, ex4);
         if(time+ex4 > dead0)
         {
            //printf("inside ex0>0\n");
            //printf("dead0 = %d, ex4 = %d, time = %d\n", dead0, ex4, time);
            t = RM_remainder(time, ex4, dead0, 0, Tasks);
         }
         else if(time+ex4 > dead1)
         {
            t = RM_remainder(time, ex4, dead1, 1, Tasks);
         }
         else if(time+ex4 > dead2)
         {
            t = RM_remainder(time, ex4, dead2, 2, Tasks);
         }
         else if(time+ex4 > dead3)
         {
            t = RM_remainder(time, ex4, dead3, 3, Tasks);
         }
         else 
         {
          //printf("inside else\n");
           t = ex4;
         }
         double e = (Tasks[4].power*pow(10,-3))*t;
         energyC = energyC + e;
         if(t != 0)
         {
         printf("%d %s %d %d %lfJ\n", time, &Tasks[4].taskName, Tasks[4].cpuFreq, t, e);
         }
         time = t + time;
         dead0 = RM_deadline(time, dead0, Tasks, 0);
         dead1 = RM_deadline(time, dead1, Tasks, 1);
         dead2 = RM_deadline(time, dead2, Tasks, 2);
         dead3 = RM_deadline(time, dead3, Tasks, 3);
         dead4 = RM_deadline(time, dead4, Tasks, 4);
         ex0 = RM_exec(ex0, Tasks, 0);
         ex1 = RM_exec(ex1, Tasks, 1);
         ex2 = RM_exec(ex2, Tasks, 2);
         ex3 = RM_exec(ex3, Tasks, 3);
         ex4 = RM_exec(ex4, Tasks, 4);
         if(time >= CPU.execution)
         {
         break;
         }
         ex4 = ex4 - t;
         t=0;
         flag = nextTask(flag, ex0, ex1, ex2, ex3, ex4);
         break;
       }
       
       case 6:
       {
       //printf("ex - %d %d %d %d %d\n", ex0, ex1, ex2, ex3, ex4);
         idleArr[0] = dead0;
         idleArr[1] = dead1;
         idleArr[2] = dead2;
         idleArr[3] = dead3;
         idleArr[4] = dead4;
         for(int b = 0; b<5; b++)
         {
          int idd = idleArr[b];
          for(int bb = 0; bb<5; bb++)
          {
            if(idd<idleArr[bb]){
            int did = idleArr[b];
            idleArr[b] = idleArr[bb];
            idleArr[bb]= did;}
          }
         }
         t = abs(idleArr[0] - time);
         idleT=idleT + t;
         //printf("idleArr - %d %d %d %d %d\n", idleArr[0], idleArr[1], idleArr[2], idleArr[3], idleArr[4]);
         double e = (CPU.idle*pow(10,-3))*t;
         energyC = energyC + e;
         printf("%d IDLE IDLE %d %lfJ\n", time, t, e);
         time = time + t;
         dead0 = RM_deadline(time, dead0, Tasks, 0);
         dead1 = RM_deadline(time, dead1, Tasks, 1);
         dead2 = RM_deadline(time, dead2, Tasks, 2);
         dead3 = RM_deadline(time, dead3, Tasks, 3);
         dead4 = RM_deadline(time, dead4, Tasks, 4);
         //printf("new dead - %d %d %d %d %d\n", dead0, dead1, dead2, dead3, dead4);
         ex0 = RM_exec(ex0, Tasks, 0);
         ex1 = RM_exec(ex1, Tasks, 1);
         ex2 = RM_exec(ex2, Tasks, 2);
         ex3 = RM_exec(ex3, Tasks, 3);
         ex4 = RM_exec(ex4, Tasks, 4);
         //printf("new ex - %d %d %d %d %d\n", ex0, ex1, ex2, ex3, ex4);
         t=0;
         flag = nextTask(flag, ex0, ex1, ex2, ex3, ex4);
         /*if(time == 200)
         {
           return 0;
         }*/
         break;
       }
        
     }
    
    }
    gettimeofday(&end, NULL);
    double totalT = 0;
    timersub(&end, &start, &res);
    totalT = (res.tv_sec * 1000000 + res.tv_usec);
    totalT = totalT/1000000;
    printf("Total energy consumed: %lfJ\n", energyC);
    float idleP = ((float)idleT/1000)*100;
    printf("Percentage of idle time:%f% \n", idleP);
    printf("Total time of schedule: %lf seconds\n", totalT);
    }
    }
    
    
    int RM_remainder(int time, int ex, int dead, int i, struct Task Tasks[])
    {
      int newEx;
      if((time+ex)>=(dead))
      {
        newEx = abs((dead)-time);
      }
      else
      {
        newEx = ex;
      }
      return newEx;
    }
    int RM_exec(int ex, struct Task Tasks[], int i)
    {
      if(Tasks[i].status == 1)
      {
        ex = ex + Tasks[i].execu;
        Tasks[i].status = 0;
      }
      else
      {
       ex = ex;
      }
      return ex;
    }
    
    int nextTask(int flag, int ex0, int ex1, int ex2, int ex3, int ex4)
    {
      int f=0;
      //printf("ex- %d %d %d %d %d\n", ex0, ex1, ex2, ex3, ex4);
      if(ex0>0)
      {
        f= 1;
      }
      else if(ex1>0)
      {
       f = 2;
      }
      else if(ex2>0)
      {
       f = 3;
      }
      else if(ex3>0)
      {
       f = 4;
      }
      else if(ex4>0)
      {
       f = 5;
      }
      else
      {
       f = 6;
      }
      return f;
    }
    
    int RM_deadline(int time, int dead, struct Task Tasks[], int i)
    {
       if((dead)<=time)
          {
            dead = dead + Tasks[i].deadline;
            Tasks[i].status = 1;
          }
       else
         {
           dead = dead;
         }
       return dead;
    }
