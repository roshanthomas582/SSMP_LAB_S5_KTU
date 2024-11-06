#include<stdio.h>
#include<stdlib.h>

int requests[100], i, j, numRequests, totalHeadMovement = 0, initialHead, diskSize, currentMovement  , posi; 

void fcfs();
void scan();
void cscan();
void read();

void main()
{
    int op;
    while (1)
    {
        printf("\n1.FCFS 2.SCAN 3.C-SCAN 4.Exit Enter Your Choice: ");
        scanf("%d",&op);
        switch (op)
        {
           case 1:
              fcfs();
              break;
           case 2:
              scan();
              break;
           case 3:
              cscan();
              break;
           case 4:
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
    int currentHeadMovement,totalHeadMovement=0;
    printf("Enter initial head position: ");
    scanf("%d", &initialHead);
    printf("Enter number of disk requests: ");
    scanf("%d", &numRequests);
    printf("Enter the disk requests: ");
    for (i = 1; i <= numRequests; i++)
        scanf("%d", &requests[i]);
    requests[0] = initialHead; 
    printf("\n---FCFS DISK SCHEDULING---\n");
    for (i = 0; i <numRequests; i++) 
    {
        currentHeadMovement = abs(requests[i] - requests[i + 1]);
        printf("Head movement from %d to %d: %d\n",requests[i],requests[i+1],currentHeadMovement);
        totalHeadMovement += currentHeadMovement;
    }
    printf("Total seek time is : %d\n", totalHeadMovement);
    printf("Average seek time is %f: \n",totalHeadMovement*1.0/numRequests);
}

void read()
{
    printf("Enter total disk size: ");
    scanf("%d", &diskSize);
    printf("Enter the number of Requests: ");
    scanf("%d", &numRequests);
    printf("Enter the Requests sequence: ");
    for (i = 0; i < numRequests; i++)
        scanf("%d", &requests[i]);
    printf("Enter initial head position: ");
    scanf("%d", &initialHead);
    for (i = 0; i < numRequests; i++)
        for (j = 0; j < numRequests - i - 1; j++)
            if (requests[j] > requests[j + 1]) 
            {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
    for (i = 0; i < numRequests; i++) 
        if (initialHead < requests[i]) 
        {
            posi = i;
            break;
        }
}

void scan()
{
    read();
    totalHeadMovement = 0;
    printf("\n---SCAN Head Movements---\n");
    for (i = posi; i < numRequests; i++) 
    {
        currentMovement = abs(requests[i] - initialHead);
        printf("From  %d to %d: %d\n",initialHead,requests[i],currentMovement);
        totalHeadMovement += currentMovement;
        initialHead = requests[i];
    }
    currentMovement = abs(diskSize - requests[i - 1] - 1);
    printf("From  %d to right end: %d\n",requests[i-1],currentMovement);
    totalHeadMovement += currentMovement;
    initialHead = diskSize-1;
    for (i = posi-1; i >= 0; i--) 
    {
        currentMovement = abs(requests[i] - initialHead);
        printf("From  %d to %d: %d\n",initialHead,requests[i],currentMovement);
        totalHeadMovement += currentMovement;
        initialHead = requests[i];
    }
    printf("Total head movement: %d\n", totalHeadMovement);
    printf("Average seek time is %f: \n",totalHeadMovement*1.0/numRequests);
}

void cscan()
{
    read();
    totalHeadMovement = 0;
    printf("\n--- C-SCAN Head Movements---\n");
    for (i = posi; i < numRequests; i++) 
    {
        currentMovement = abs(requests[i] - initialHead);
        printf("From  %d to %d: %d\n",initialHead,requests[i],currentMovement);
        totalHeadMovement += currentMovement;
        initialHead = requests[i];
    }
    currentMovement = abs(diskSize - requests[i - 1] - 1);
    printf("From  %d to right end: %d\n",requests[i-1],currentMovement);
    totalHeadMovement += currentMovement;
    printf("From  right end to start: %d\n",abs(diskSize - 1));
    totalHeadMovement += abs(diskSize - 1);
    initialHead = 0;
    for (i = 0; i < posi; i++) 
    {
        currentMovement = abs(requests[i] - initialHead);
        printf("From   %d to %d: %d\n",initialHead,requests[i],currentMovement);
        totalHeadMovement += currentMovement;
        initialHead = requests[i];
    }
    printf("Total head movement: %d\n", totalHeadMovement);
    printf("Average seek time is %f: \n",totalHeadMovement*1.0/numRequests);

}