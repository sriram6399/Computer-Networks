// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>

#define PORT 10010
#define MAXLINE 1024
#define MSIZE 200

int sendtime[MSIZE];
//int recvtime = malloc(sizeof(int)*MSIZE);
int sockfd;
struct sockaddr_in servaddr;

void *sendData()
{
int i ;
char hello[50];
struct timeval start[MSIZE];
for(i = 0 ;i<MSIZE; i++ ) 
{
	sprintf(hello ,"%d", i+1);
	sendto(sockfd, (const char *)hello, strlen(hello),0, (const struct sockaddr *) &servaddr,sizeof(servaddr));
	gettimeofday(&start[i],NULL);
	printf("Message sent to server .\n");
	printf("time at which %d  is sent is %d \n",i+1,start[i].tv_usec);
	sendtime[i] = start[i].tv_usec;
}
}

void *recvData()
{
int j,n,l,k=0,len;
char buffer[50];
struct timeval end[MSIZE];
int missing[MSIZE];
for(j = 0 ;j<MSIZE ; j++ ) 
{
	if((n = recvfrom(sockfd, (char *)buffer,MAXLINE, 0 , (struct sockaddr *) &servaddr,&len))<=0)
	{
	 printf("Read Error on Client");
	}
	buffer[n] = 0;
	int i = atoi(buffer)-1 ;
	gettimeofday(&end[i],NULL);
	printf("time at which %d is recieved is %d \n",i+1,end[i].tv_usec);
	
	printf("Echo from Server : %s\n", buffer);
	//recvtime[i] = start[i].tv_usec;
}
int max = 0 , min = INT_MAX;
int sumavg=0 ,avg ;
for(l=0 ; l<MSIZE  ;l++)
{
	 if(sendtime[l] == 0 || end[l].tv_usec == 0 )
	 {
		printf("%d is missing \n" ,l+1);
		missing[k++] = l+1;
		
	 }
	else
	{
		if((end[l].tv_usec - sendtime[l] )<0)
		{
			continue ;
		}
		sumavg = sumavg + end[l].tv_usec - sendtime[l] ;
		if((end[l].tv_usec - sendtime[l] ) < min)
		{
			min = (end[l].tv_usec - sendtime[l] );
		}
		if((end[l].tv_usec - sendtime[l] ) > max)
		{
			max = (end[l].tv_usec - sendtime[l] );
		}
	}
}
	avg = sumavg /(MSIZE - k) ;
	if(k>0){
	printf("Missing echoes are \n");
	for(l=0 ; l< k ;l++ )
	{
	printf("%d\t",missing[l]);
	}
	}
	printf("\n");
	printf("Highest RTT in microseconds is  = %d , Lowest RTT in microseconds is =%d  , Average RTT in microseconds is = %d \n" ,max,min,avg);
	
}
void main(int argc , char **argv)
{
pthread_t se;
pthread_t re;
if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

memset(&servaddr, 0, sizeof(servaddr));

servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(PORT);
servaddr.sin_addr.s_addr = inet_addr(argv[1]);

pthread_create(&se, NULL, sendData(), NULL);
pthread_create(&re, NULL, recvData(), NULL);

 pthread_exit(NULL);
close(sockfd);
//return 0;
}