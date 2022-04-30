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

int main (int argc, char **argv)
{
 int listenfd, connfd, n;
 socklen_t clilen;
 char buf[32];
 struct sockaddr_in cliaddr, servaddr;
 int i,s,t,bi,ai;
 char a[32];
 char b[32];
 char f[64]; // back to 64
 char r[64];
 char c;
 char eq[2];
 
 int v = 0;
 int x;
 int u = 0;
int answer;
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
  // printf("%s","String received from the client:");
   buf[n]=0;
   //a[n] = 0;
   //b[n] = 0;
   //f[n] = 0;
   counter = 0;
  // printf("%s", buf);
   for (i = 0; i < n; i++) {
   if (!isdigit(buf[i])) {   
   c = buf[i]; 
   counter = 1;
   }
   if (isdigit(buf[i]) && counter == 0) {
   a[0] = buf[i];
   }
   if (isdigit(buf[i]) && counter == 1) {
   b[0] = buf[i];
   break;
   }
    // i is pointer to c
   //break; //break for loop
  // a = memcpy(buff, a, i - 1); // i is location of symbol
  // b = memcpy(buff, b, i + 1);
  // char *p = i;
  // strcpy(i, b);
  // c = buff[i];
   } /*
   
   for (s = 0; s < i; s++) { // make a string from chars before operation(c)
   a[v] = buf[s];
   v++;
   }
   
   for (t = i+1; t < strlen(buf); t++) {// make b string from chars before operation(c)
   b[u] = buf[t];
   u++;
   } */
   
   ai = atoi(a);
   bi = atoi(b);
   
   switch(c) { // calculate answer
   case '+':
   answer = ai + bi;
   //printf("\nperformed a+b");
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
 if (answer != answer || x ==1)
{
 eq[0] = '!';
 eq[1] = '=';
 answer = 0;
}
 sprintf(f, "%s%s%d",buf,eq,answer);
   
   //f[n] = 0;
  // puts(f);
   
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