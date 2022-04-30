#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <limits.h>
#include <pthread.h> 
#include <time.h>
#include <math.h>

int finish = 0; 

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
struct queue{
    struct packet *arr_pac[10000];
    int start;
    int end;
    int size;
};
struct packet{
    int val;
    time_t start_time;
    double service_time;
};
struct queue *initialiseQueue(){
   struct queue *q = (struct queue*)malloc(sizeof(struct queue));
   q->start = 0;
   q->end = 0;
   q->size=0;
   return q;
}

struct process1{
int method;
double lambda;
struct queue *q1;
struct queue *q2;
};

struct process2{
struct queue *q;
double u;
};

struct process1* queueenv(int method, double lambda, struct queue *q1, struct queue *q2)
{
    struct process1 *temp = (struct process1*)malloc(sizeof(struct process1));
    temp->method = method;
    temp->lambda = lambda;
    temp->q1 = q1;
    temp->q2 = q2;
return temp;
}
struct process2* serverenv(double u, struct queue *q)
{
    struct process2 *temp = (struct process2*)malloc(sizeof(struct process2));
    temp->u = u;
    temp->q = q;
return temp;
}
int enque(struct queue *q){
if (q->size < 10) 
{
  struct packet *temp ;
  temp->start_time = time(NULL);
  temp->val = 1;
  q->arr_pac[q->end] = temp;
  q->end=q->end +1;
  q->size = q->end - q->start;
  return q->size -1 ;
}
else{
return -1;
}
}
struct packet *deque(struct queue *q) {
if(q->size >0){
  struct packet *temp ;
  temp=q->arr_pac[q->start];
  temp->service_time = time(NULL) - temp->start_time;
  q->start=q->start +1;
  q->size = q->end - q->start;
  return temp;
}
return NULL;
}
int randomSelection(struct queue *q1, struct queue *q2){
if(rand()%2 ==0){
return enque(q1);
}
else{
return enque(q2);
}
}
int minQueue(struct queue *q1, struct queue *q2){
 if(q1->size > q2->size){
 return enque(q2);
}
else{
return enque(q1);
}
}
void *queueprocess(struct process1 *params){
    double blockCounter = 0.0;
    double blockPro = 0.0;
    double queLenCounter = 0.0;
    double averageQueueLen = 0.0;
    int i = 0;
  if (params->method == 0) 
    {
        for (i = 0; i < 10000; i++)
        {
            double interval = exponGenerator(params->lambda);
            sleep(interval);
            int res = randomSelection(params->q1, params->q2);
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
    else if (params->method == 1)
    {
        for (i = 0; i < 10000; i++)
        {
            double interval = exponGenerator(params->lambda);
            sleep(interval);
            int res = minQueue(params->q1, params->q2);
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
    blockPro = blockCounter/10000;
    averageQueueLen = queLenCounter/10000;
    printf("Blocking Probability is : %f",&blockPro);
    printf("Average Que Length is : %f",&blockPro);
    finish = 1;
    

}
void *serverprocess(struct process2 *serverparams) 
{
 struct packet processed_arr[10000];
 int i=0,j;
    while(1)
    {
        if (finish == 1 && serverparams->q->size == 0) 
        {
            break;
        }
        else
        {
            double interval = exponGenerator(serverparams->u);
            sleep(interval);
            struct packet *k = deque(serverparams->q);
	    if(!k){
            processed_arr[i]=*k;
		    i++;
	    }

        }
    }
float sourjanCounter=0;
float sourjan;
for(j=0;j<i;j++){
sourjanCounter = sourjanCounter+ processed_arr[j].service_time;
}
sourjan = sourjanCounter/(i-1);
printf("Average Sourjan Time is : %f ", &sourjan);
}
void *threadone(void * k){
    double lambda = 5;
    struct queue *q1 = initialiseQueue();
    struct queue *q2 = initialiseQueue();
    struct process1 *queueparams = queueenv(0, lambda, q1, q2);
    queueprocess(queueparams);
}
void *threadtwo(void * k){
    double servicerate = 5;
    struct queue *q1 = initialiseQueue();
    struct process2 *server1params = serverenv(servicerate, q1);
    serverprocess(server1params);
}
void *threadthree(void * k){
    double servicerate = 5;
    struct queue *q2 = initialiseQueue();
    struct process2 *server2params = serverenv(servicerate, q2);
    serverprocess(server2params);
}
int main(int argc, char * argv[]) 
{
    int i = 0;
    /*if (argc != 3)
    {
        printf("Incorrect parameters!\n");
        return 0;
    }*/
    
    
    for (i = 0; i < 1; i++)
    {
        pthread_t thread1, thread2, thread3;
        pthread_create(&thread3, NULL, threadthree, NULL );
        pthread_create(&thread2, NULL, threadtwo, NULL);
        pthread_create(&thread1, NULL, threadone, NULL); 
    }
}