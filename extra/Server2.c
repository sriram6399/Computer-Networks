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
#define LOCALHOST 2130706433

int main(int argc, char **argv){
int listenfd, connfd, n;
socklen_t clilen;
char buf[MAXLINE] ; //buffer for data send and recv
//char * adr ="131.204.14.247";
struct sockaddr_in cliaddr, servaddr;//server and client socket addresses
//creation of the socket
listenfd = socket (AF_INET, SOCK_STREAM, 0);
//preparation of the socket address 
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//kernel chooses source ip address(any interface of the host)
servaddr.sin_port = htons(SERV_PORT); //host to network byte order conversion
bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
listen(listenfd, LISTENQ);
printf("%s\n","Server running...waiting for connections.");
for( ; ; ) {
clilen = sizeof(cliaddr);
connfd = accept(listenfd, (struct sockaddr *) &cliaddr, (socklen_t *) &clilen);//get the new socket
printf("%s\n","Received request...");
while( (n = recv(connfd, buf, MAXLINE,0)) > 0) 
{ //start receiving on the data sock
printf("%s","String received from and resent to the client:");
buf[n]=0;  //ending flag for the string
puts(buf);
send(connfd, buf, n, 0); //echo it back to the client
}
if(n < 0) 
{
perror("Read error"); 
exit(1);
}
close(connfd); //finish communication with one client; go back and wait for next client
}
//close listening socket
close(listenfd); 
}