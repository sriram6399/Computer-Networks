#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 10010 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

int main(){
int sockfd, n;
char *buf = malloc(sizeof(char) * MAXLINE);
struct sockaddr_in cliaddr, servaddr;//server and client socket addresses

//creation of the socket

if((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) <0)
{
perror("Socket not connected properly");
exit(1);
}

memset(&servaddr, 0, sizeof(servaddr));
memset(&cliaddr, 0, sizeof(cliaddr));

//preparation of the socket address 

servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //kernel chooses source ip address(any interface of the host)
servaddr.sin_port = htons(SERV_PORT) ; //host to network byte order conversion

if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)
{
perror("Bind not connected properly");
exit(2);
}

printf("\n Server running...waiting for connections. \n");

//for( ; ; ) {
int clilen = sizeof(cliaddr);

while( (n = recvfrom(sockfd, (char *) buf, MAXLINE, 0, ( struct sockaddr *) &cliaddr,&clilen)) > 0) 
{ 
//start receiving on the data sock
printf("%s/n","String received from and resent to the client:");
buf[n]=0;  //ending flag for the string
puts(buf);
sendto(sockfd, (const char *) buf, n ,  0 , (const struct sockaddr *) &cliaddr, clilen); //echo it back to the client
printf("echo sent back");
}
if(n <= 0) 
{
perror("Read error"); 
exit(3);
}

//}
close(sockfd);
}