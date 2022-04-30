#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 10010 /*port*/
#define LOCALHOST 2130706433

int main(int argc , char **argv) //user specifies server ip address in command line int argc, char **argv
{
int sockfd;
struct sockaddr_in servaddr;
//char * adr ="131.204.14.247";
char  *sendline = malloc(sizeof(char) * MAXLINE);
char  *recvline= malloc(sizeof(char) * MAXLINE);//basic check of the arguments
//additional checks can be inserted
if(argc !=2) {
perror("Usage: echoClient IP address of the server"); 
exit(1);
}
if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) 
{
perror("Problem in creating the socket");
exit(2);
}//Creation of the socket address
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr= htonl(LOCALHOST); //convert number_dot string to binary
servaddr.sin_port =  htons(SERV_PORT); //convert to network byte order//Connection of the client to the socket
if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) 
{
perror("Problem in connecting to the server");
exit(3);
}
while(fgets(sendline, MAXLINE, stdin) != NULL) 
{
send(sockfd, sendline, strlen(sendline), 0); // send input to server
if(recv(sockfd, recvline, MAXLINE,0) == 0)
{ 
perror("The server terminated prematurely");
exit(4);
}
printf("%s", "String received from the server: ");
fputs(recvline, stdout);
}
exit(0);
}