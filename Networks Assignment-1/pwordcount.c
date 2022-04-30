#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <stdlib.h>
 
#define BUFFER_SIZE 10
#define READ_END 0 
#define WRITE_END 1 

void send_data(FILE *file, int fd[2], int pd[2]){ //method in the parent process

char str[BUFFER_SIZE]; 
char no_words[BUFFER_SIZE];

close(fd[READ_END]);   


printf("\n Process 1 is reading file now..... \n");
printf("\n Process 1 starts sending data to Process 2 ... \n");

 while (fgets(str, BUFFER_SIZE, file) != NULL) {  // reading a file and writing the data to pipe1
        write(fd[WRITE_END], str, BUFFER_SIZE);
    }

 close(fd[WRITE_END]); 
 close(pd[WRITE_END]);
read(pd[0], no_words, BUFFER_SIZE);  // reading the count of words from pipe 2 and printing it.
printf("\n Process1: No of Words is %s \n",no_words);  
close(pd[READ_END]);
}

void word_count(int fd[2],int pd[2] ){ 
close(fd[WRITE_END]); 
int bytes;
char inbuf[BUFFER_SIZE]; 
char ch;
int words=0;
int foundLetter = 0;
while ((bytes = read(fd[0], inbuf, BUFFER_SIZE))>0){  // reading the words from pipe1 and counting it
	int i=0;
        
      while(i<bytes){
	ch=inbuf[i];  
   if (ch == ' ')
      foundLetter = 0;
  else 
  {    
      if (foundLetter == 0){
          words++;}
      foundLetter = 1;
  }
	i++;
	}
   }
printf("\n Process 2 finishes receiving data from Process 1 ... \n");
printf("\n Process 2 is counting words now ...  \n");

char no_of_words[BUFFER_SIZE];
sprintf(no_of_words, "%d", words);

close(fd[READ_END]);
close(pd[READ_END]);
printf("\n Process 2 is sending the result back to Process 1 ...  \n");
write(pd[WRITE_END], no_of_words, BUFFER_SIZE);  // sending the no of words to parent process through pipe2
close(pd[WRITE_END]);

}

int main(int argc, char *argv[]) 
{ 
FILE * file;

if(argc !=2){
 printf("\n Please Provide the file name \n Usage : ./pwordcount <file_name> \n");
 return 0;
}
 pid_t pid; 

int fd[2];
int pd[2];

 /* create the pipe */ 
 if (pipe(fd) == -1 || pipe(pd) == -1) { 
  fprintf(stderr,"Pipe failed"); 
  return 1; 
 } 
 
file = fopen(argv[1], "r"); //trying to open a file
   if (file == NULL){
      printf("\n Unable to open file. Please check the file name \n");
      exit(EXIT_FAILURE);
   }

 pid = fork(); //fork creation

 if (pid < 0) { 
  fprintf(stderr, "Fork failed"); 
  return 1; 
 } 
 
 if (pid > 0) {  /* parent process */ 
 
 send_data(file,fd,pd);
 } 
 else { /* child process */ 
   word_count(fd,pd);
 } 
 
 return 0; 
} 
 