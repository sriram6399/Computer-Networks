#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

int main ()
{
 int listenfd, connfd, n;
 socklen_t clilen;
 char buf[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;
 int i,s,t;
 uint32_t bi,ai;
 char f[64]; // back to 64
 char c;
 char eq[2];
 
 int v = 0;
 int x;
 int u = 0;
unsigned int answer;
int counter;
 //float answer;
	
 //creation of the socket
 listenfd = socket (AF_INET, SOCK_STREAM, 0);
	
 //preparation of the socket address 
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(SERV_PORT);
	
 bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
 listen(listenfd, LISTENQ);
	
 printf("%s\n","Server running...waiting for connections.");
	
 for ( ; ; ) {
 
		
  clilen = sizeof(cliaddr);
  connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
  //printf("Received request...\n");
				
  while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
 
   buf[n]=0;
  
   counter = 0;
  answer = 0;
 
   printf("%u is buf[1]",buf[1]);
   
   ai = (buf[1]<<24)+(buf[2]<<16)+(buf[3]<<8)+(buf[4]);
   bi = (buf[5]<<24)+(buf[6]<<16)+(buf[7]<<8)+(buf[8]);
   c= buf[0];

   printf("%u is a value", ai);
   printf("%u is b value", bi);
   printf("%c is c value", c);
   
   switch(c) { // calculate answer
   case '+':
   answer = ai + bi;
  break;
   
   case '-':
    
    answer = ai - bi;    
    
   // printf("\nperformed a-b");
    break;
   case 'x':
    answer = ai * bi;
   // printf("\nperformed a*b");
    break;
   case '/':
   if (bi != 0) { //valid answer
    answer = ai / bi;
    }    else { // invalid answer
    answer = 0;
    x = 1;
    }
    // printf("\nperformed a/b");
    break;
    default:
      printf("\nError: Unrecognized operator");
      }
   eq[0] = '=';
 if (answer != answer || x ==1 || answer < 0)
{
 eq[0] = '!';
 eq[1] = '=';
 answer = 0;
}
 sprintf(f, "%s%s%u",buf,eq,answer);
   
  puts(f);
   
   send(connfd, f, strlen(f), 0); // N VALUE WAS ISSUE
  }
			
 if (n < 0) {
  perror("Read error"); 
  exit(1);
 }
 close(connfd);
	
 }
 //close listening socket
 close(listenfd); 
}