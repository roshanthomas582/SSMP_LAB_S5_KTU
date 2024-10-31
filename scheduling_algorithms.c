#include<stdio.h>
#include<stdlib.h>
void fcfs();
void sjf();
void rr();
void prio();
void main()
{
    int op;
    while (1)
    {
        printf("\n\n1.FCFS 2.SJF 3.Round Robin 4.Priority 5.Exit Enter Your Choice: ");
        scanf("%d",&op);
        switch (op)
        {
           case 1:
              fcfs();
              break;
           case 2:
              sjf();
              break;
           case 3:
              rr();
              break;
           case 4:
              prio();
              break;
           case 5:
              exit(0);
              break;
           default:
              printf("Wrong Choice!");
              break;
        }
    }
}
void fcfs()
{
   int no;
   float sumtat=0 , sumwt=0;
   printf("Enter the number of processes: ");
   scanf("%d",&no);
   int btime[no] , tatime[no] , wtime[no] ,artime[no],pid[no];
   for (int i = 0; i < no; i++)
   {
      printf("Enter the Burst Time of Process P[%d]: ",i);
      scanf("%d",&btime[i]);
      printf("Enter the Arrival Time of Process P[%d]: ",i);
      scanf("%d",&artime[i]);
      pid[i]=i;
   }
   for (int i = 0; i < no; i++)
      for (int j = 0; j < no-i-1; j++)
         if (artime[j]>artime[j+1])
         {
            int temp=artime[j];
            artime[j]=artime[j+1];
            artime[j+1]=temp;

            temp=btime[j];
            btime[j]=btime[j+1];
            btime[j+1]=temp;

            temp=pid[j];
            pid[j]=pid[j+1];
            pid[j+1]=temp;
         }
   wtime[0]=0 , tatime[0]=btime[0];
   sumtat=sumtat+tatime[0];
   for (int i = 1; i < no; i++)
   {
      wtime[i]=wtime[i-1]+btime[i-1];
      tatime[i]=wtime[i]+btime[i];  
      sumtat+=tatime[i];
      sumwt+=wtime[i]; 
   }
   printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\tArrival Time");
   for (int i = 0; i < no; i++)
      printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d",pid[i],btime[i],wtime[i],tatime[i],artime[i]);
   printf("\nAverage Waiting Time is: %f",sumwt/no);
   printf("\nAverage Turn Around Time is: %f",sumtat/no);
   printf("\nGanttchart");
   printf("\n= = = = = = = = = = = = = = = = =\n");
   for (int i = 0; i < no; i++)
      printf("  P[%d]\t",pid[i]);
   printf("\n= = = = = = = = = = = = = = = = =\n");
   int temp=0;
   for (int i = 0; i <= no; i++)
   {
      printf("%d\t",temp);
      temp+=btime[i];
   }
}
void sjf()
{
   int no;
   float sumtat=0 , sumwt=0;
   printf("Enter the number of processes: ");
   scanf("%d",&no);
   int btime[no] , tatime[no] , wtime[no] , pid[no];
   for (int i = 0; i < no; i++)
   {
      printf("Enter the Burst Time of Process P[%d]: ",i);
      scanf("%d",&btime[i]);
      pid[i]=i;
   }
   for (int i = 0; i < no; i++)
      for (int j = 0; j < no-i-1; j++)
         if (btime[j]>btime[j+1])
         {
            int temp=btime[j];
            btime[j]=btime[j+1];
            btime[j+1]=temp;

            temp=pid[j];
            pid[j]=pid[j+1];
            pid[j+1]=temp;
         }
   wtime[0]=0 , tatime[0]=btime[0];
   sumtat=sumtat+tatime[0];
   for (int i = 1; i < no; i++)
   {
      wtime[i]=wtime[i-1]+btime[i-1];
      tatime[i]=wtime[i]+btime[i];  
      sumtat+=tatime[i];
      sumwt+=wtime[i]; 
   }
   printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time");
   for (int i = 0; i < no; i++)
      printf("\nP[%d]\t\t%d\t\t%d\t\t%d",pid[i],btime[i],wtime[i],tatime[i]);
   printf("\nAverage Waiting Time is: %f",sumwt/no);
   printf("\nAverage Turn Around Time is: %f",sumtat/no);
   printf("\nGanttchart");
   printf("\n= = = = = = = = = = = = = = = = =\n");
   for (int i = 0; i < no; i++)
      printf("  P[%d]\t",pid[i]);
   printf("\n= = = = = = = = = = = = = = = = =\n");
   int temp=0;
   for (int i = 0; i <= no; i++)
   {
      printf("%d\t",temp);
      temp+=btime[i];
   }
}
void rr()
{
   int i, limit, total = 0, x, counter = 0, time_quantum , j=0 , k=0,sum=0;
   int wait_time = 0, turnaround_time = 0, burst_time[10], temp[10] , gc[50] , id[50];
   float average_wait_time, average_turnaround_time;
   printf("Enter Total Number of Processes: ");
   scanf("%d", &limit);
   x  = limit;
   for(i = 0; i < limit; i++) 
   {
       printf("Enter Burst Time of Process P[%d]: ", (i + 1));
       scanf("%d", &burst_time[i]);
       temp[i] = burst_time[i];
   }
   printf("Enter Time Quantum: ");
   scanf("%d", &time_quantum);
   printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");
   for(total = 0, i = 0; x != 0;) 
   {
       if(temp[i] <= time_quantum && temp[i] > 0) 
       {
           total = total + temp[i];
           gc[j++]=temp[i];
           id[k++]=(i+1);
           temp[i] = 0;
           counter = 1;
       }
       else if(temp[i] > time_quantum) 
       {
           temp[i] = temp[i] - time_quantum;
           gc[j++]=time_quantum;
           id[k++]=(i+1);
           total = total + time_quantum;
       }
       if(temp[i] == 0 && counter == 1) 
       {
           x--;
           printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", i + 1, burst_time[i], total, total - burst_time[i]);
           wait_time = wait_time + total - burst_time[i];
           turnaround_time = turnaround_time + total ;
           counter = 0;
       }
       if(i == limit - 1) 
           i = 0;
       else 
           i++;
   }
   printf("\n= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
   for (i = 0; i < k; i++)
      printf("  P[%d]\t",id[i]);
   printf("\n= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
   int temp12=0;
   for (int m = 0; m <= j; m++)
   {
      printf("%d\t",temp12);
      temp12+=gc[m];
   }
   average_wait_time = wait_time * 1.0 / limit;
   average_turnaround_time = turnaround_time * 1.0 / limit;
   printf("\n\nAverage Waiting Time:\t%f", average_wait_time);
   printf("\nAvg Turnaround Time:\t%f\n", average_turnaround_time);
}
void prio()
{
   int no;
   float sumtat=0 , sumwt=0;
   printf("Enter the number of processes: ");
   scanf("%d",&no);
   int btime[no] , tatime[no] , wtime[no] , pid[no] , prio[no];
   for (int i = 0; i < no; i++)
   {
      printf("Enter the Burst Time of Process P[%d]: ",i);
      scanf("%d",&btime[i]);
      printf("Enter the Priority of Process P[%d]: ",i);
      scanf("%d",&prio[i]);
      pid[i]=i;
   }
   for (int i = 0; i < no; i++)
      for (int j = 0; j < no-i-1; j++)
         if (prio[j]>prio[j+1])
         {
            int temp=btime[j];
            btime[j]=btime[j+1];
            btime[j+1]=temp;

            temp=pid[j];
            pid[j]=pid[j+1];
            pid[j+1]=temp;

            temp=prio[j];
            prio[j]=prio[j+1];
            prio[j+1]=temp;
         }
   wtime[0]=0 , tatime[0]=btime[0];
   sumtat=sumtat+tatime[0];
   for (int i = 1; i < no; i++)
   {
      wtime[i]=wtime[i-1]+btime[i-1];
      tatime[i]=wtime[i]+btime[i];  
      sumtat+=tatime[i];
      sumwt+=wtime[i]; 
   }
   printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\t  Priority");
   for (int i = 0; i < no; i++)
      printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d",pid[i],btime[i],wtime[i],tatime[i],prio[i]);
   printf("\nAverage Waiting Time is: %f",sumwt/no);
   printf("\nAverage Turn Around Time is: %f",sumtat/no);
   printf("\nGanttchart");
   printf("\n= = = = = = = = = = = = = = = = =\n");
   for (int i = 0; i < no; i++)
      printf("  P[%d]\t",pid[i]);
   printf("\n= = = = = = = = = = = = = = = = =\n");
   int temp=0;
   for (int i = 0; i <= no; i++)
   {
      printf("%d\t",temp);
      temp+=btime[i];
   }
}