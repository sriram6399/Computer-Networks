#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <limits.h>
#include <windows.h> 
#include <time.h>
#include <math.h>

int finish = 0; 
int mess_size=15;
HANDLE ghMutex;

double exponGenerator(double myLamda)
{
    double pv = 0.0;
	while (1)
	{
		pv = (double)rand() / (double)RAND_MAX;
		if (pv != 1)
			break;
	}
	pv = (-1.0 / myLamda) * log(1 - pv);
	return pv;
}

struct packet{
    int val;
    time_t start_time;
    double service_time;
};
struct queue{
    struct packet arr_pac[10000];
    int start;
    int end;
    int size;
};

double lambda = 5;
double servicerate = 5;
struct queue q1 ;
struct queue q2; 

struct queue initialiseQueue(){
   struct queue q ;
   q.start = 0;
   q.end = 0;
   q.size=0;
   return q;
}

struct process1{
int method;
double lambda;
struct queue Q1;
struct queue Q2;
};

struct process2{
struct queue q;
double u;
};

struct process1 queueenv(int method)
{
    struct process1 temp ;
    temp.method = method;
    temp.lambda = lambda;
    temp.Q1 = q1;
    temp.Q2 = q2;
return temp;
}

struct process2 serverenv(double u, struct queue q)
{
    struct process2 temp;
    temp.u = u;
    temp.q = q;
return temp;
}

int enque(struct queue q){
DWORD dwWaitResult;
 dwWaitResult = WaitForSingleObject( 
            ghMutex,    // handle to mutex
            INFINITE);  // no time-out interval
 
switch (dwWaitResult) 
{
            
case WAIT_OBJECT_0:
if (q.size < 10) 
{ 
  
  struct packet temp ;
  temp.start_time = time(NULL);

  temp.val = 1;
  q.arr_pac[q.end] = temp;

  q.end = q.end +1;

  q.size = q.end - q.start;
  printf("in enque  %d",q.end);
  return q.size  ;
}
else{
return -1;
}
break; 
case WAIT_ABANDONED:
printf("abonded");
break;
case WAIT_FAILED:
printf("failed");
}
return -1;
}

struct packet deque(struct queue q) {
struct packet temp ;
DWORD dwWaitResult;
printf(" in deque %d ",q.end);
dwWaitResult = WaitForSingleObject( 
            ghMutex,    // handle to mutex
            INFINITE);  // no time-out interval
 
switch (dwWaitResult) 
{
            // The thread got ownership of the mutex
case WAIT_OBJECT_0: 
if(q.size >0){
  printf("entered");
  temp=q.arr_pac[q.start];
  temp.service_time = difftime(time(NULL),temp.start_time);
  q.start=q.start +1;
  q.size = q.end - q.start;
  return temp;
}
else{
printf("entered 2");
temp.service_time=0 ;
return temp;
} 
break; 
case WAIT_ABANDONED:
printf("abonded");
break;
case WAIT_FAILED:
printf("failed");
}
printf("entered 3");
temp.service_time=0 ;
return temp;
}

int randomSelection(){
if(rand()%2 ==0){
return enque(q1);
}
else{
return enque(q2);
}
}

int minQueue(){
 if(q1.size > q2.size){
 return enque(q2);
}
else{
return enque(q1);
}
}

void queueprocess(struct process1 params){
    double blockCounter = 0.0;
    double blockPro = 0.0;
    int queLenCounter = 0;
    int averageQueueLen = 0;
    int i = 0;
 
  if (params.method == 0) 
    {
printf(" this is the %d =0",params.method);
        for (i = 0; i < mess_size ; i++)
        {
            
            double interval = exponGenerator(params.lambda);
            sleep(interval);
            int res = randomSelection();
            
            if (res == -1) 
            { 
                blockCounter++;
                queLenCounter = queLenCounter +10;
            }
	    else{

		   queLenCounter = queLenCounter + res;
                   //printf(" %d ",queLenCounter);
		}
        }
    }
    else if (params.method == 1)
    { 
printf(" this is the %d =1  ",params.method);
        for (i = 0; i < mess_size ; i++)
        {
            double interval = exponGenerator(params.lambda);
            sleep(interval);
            int res = minQueue();
            printf("  %d mn",q1.end);
            if (res == -1) 
            {
                blockCounter++;
                queLenCounter = queLenCounter +10;
            }
	    else{
		   queLenCounter = queLenCounter +res;
		}
        }
    }
    blockPro = blockCounter/mess_size;
    averageQueueLen = queLenCounter/mess_size;
    printf("Blocking Probability is : %f",&blockPro);
    printf("Average Que Length is : %d",averageQueueLen);
    finish = 1;
    return;

}
void serverprocess(struct process2 serverparams) 
{
 struct packet processed_arr[10000];
 int i=0,j;
    while(1)
    { 
        if (finish == 1 && serverparams.q.size == 0) 
        {
            break;
        }
        else
        {
            double interval = exponGenerator(serverparams.u);
 
            sleep(interval);
            
            struct packet k = deque(serverparams.q);
             
	    if(!k.service_time){
 
            processed_arr[i]=k;

		    i++;
	    }
        }
    }
float sourjanCounter=0;
float sourjan;
for(j=0;j<i;j++){
printf(" %f",processed_arr[j].service_time);
sourjanCounter = sourjanCounter+ processed_arr[j].service_time;
}
sourjan = sourjanCounter/(i-1);
printf("Average Sourjan Time is : %f ", &sourjan);
}

DWORD WINAPI threadone(){
    printf(" thread one ");
    struct process1 queueparams = queueenv(1);
   
    queueprocess(queueparams);
}

DWORD WINAPI threadtwo(){  
    struct process2 server1params = serverenv(servicerate, q1);
    serverprocess(server1params);
}

DWORD WINAPI threadthree(){
    struct process2 server2params = serverenv(servicerate, q2);
    serverprocess(server2params);
}

int main(int argc, char * argv[]) 
{
    HANDLE hThrds[3];
    int i = 0;

    /*if (argc != 3)
    {
        printf("Incorrect parameters!\n");
        return 0;
    }
    double lambda = atof(argv[1]);
    double servicerate = atof(argv[2]);*/

    for (i = 0; i < 1; i++)
    {
        DWORD thread1,thread2,thread3;
        hThrds[2]=CreateThread(NULL,0,  threadone, NULL,0,&thread1);
        hThrds[0]=CreateThread(NULL,0, threadtwo, NULL,0,&thread2);
        hThrds[1]=CreateThread(NULL,0, threadthree, NULL,0,&thread3); 
        WaitForMultipleObjects(3, hThrds,TRUE, INFINITE);
    }
    
    for( i=0; i < 3; i++ )
        CloseHandle(hThrds[i]);

    CloseHandle(ghMutex);
}