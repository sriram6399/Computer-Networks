#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv) 
{
 int sockfd, n;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];
 unsigned int a,b;
 char c;
 //char string[MAXLINE];
 
 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server>"); 
  exit(1);
 }
	
 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }
	
 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
   
    // ask for a unsigned int
   printf("\nEnter an unsinged int: ");
   scanf(" %d", &a);
   // ask for b unsigned int
   printf("\nEnter an unsinged int: ");
   scanf(" %d", &b);
   // ask for c char operation
   printf("\nEnter a four function operation: ");
   scanf(" %c", &c);
   //package into string or char array
   sprintf(sendline, "%d%c%d",a,c,b);
   //printf(" %s", &sendline);
   
 //Connection of the client to the socket 
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
	
  
// while (fgets(sendline, MAXLINE, stdin) != NULL) { // remove loop at end
	
  send(sockfd, sendline, strlen(sendline), 0);
		
  if ((n=recv(sockfd, recvline, MAXLINE,0)) == 0){
   //error: server terminated prematurely
   perror("The server terminated prematurely"); 
   exit(4);
  }
  printf("%s", "\nString received from the server: "); // change this to unsigned integer
  //puts(recvline);
  //printf("%s", recvline);
  recvline[n]=0;
  puts(recvline);
// }

 exit(0);
}
