#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_LINE_SIZE 128
#define MAX_BUF_SIZE 1280
char buf[MAX_BUF_SIZE];

// where is the text input string ?
// what is the expected out put ?
// There is already a exit(0) out there. 
// explain code to TA/ professor.
void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

//create a new process
int
fork1(void)
{
  int pid;
  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

//create a pipe
void 
pipe1(int fd[2])
{
 int rc = pipe(fd);
 if(rc<0){
   panic("Fail to create a pipe.");
 }
}


//pull everything from pipe and return the size of the content
int
read1(int *fd)
{
 char ch='a';
 write(fd[1],&ch,1); //write something to the pipe so the next read will not block (because read operation is a blocking operation - if the pipe is empty, read will wait till something is put in)
 int len = read(fd[0],buf,MAX_BUF_SIZE)-1;
 return len;
}


int
main(int argc, char *argv[])
{
  // Check if the input is correct.
  if(argc == 3){
    printf("Please provide correct input");
    exit(1);
  }
    int argv1 = atoi(argv[1]);
    int argv2 = atoi(argv[2]);
// check if arguement 1 is multiple of argument2.
    if(argv1%argv2!=0){
        printf("A must be multuple of B");
        exit(1);
    }
  //create two pipe to share with child
  int fd1[2], fd2[2];
  pipe1(fd1); //a pipe from child to parent - child sends entered texts to parent for counting
  pipe1(fd2); //a pipe from child to parent - child lets parent stop (when user types :exit)

  int result = fork1();//create child process
  if(result==0){

     //child process:

     close(fd1[0]);
     close(fd2[0]);
    // char buf[MAX_LINE_SIZE];

     int timer =0; // initiate timer =0
     while(1){
        sleep(argv2);
        timer += argv2;
       //write(fd1[1],buf,strlen(buf)-1);//-1 because we remove the end /0 from string

        char ch2[] ="hello!";
        write(fd1[1],ch2,6); // write in to the pipe.
       //check for exit
       if(timer == argv1){
          char ch='a';
	        write(fd2[1],&ch,1);// tell parent to exit 
          exit(0);
       }  
     }
  }else{

     //parent process:

     // if the timer runs at the maxium time(A time)m then it will quit while loop.
     while(1){
       sleep(60);

       int len=read1(fd1); 	
       printf("\nIn last minute, %d characters were entered.\n", len);

       len=read1(fd2);
       if(len>0){
          //now to terminate
          wait(0);
          exit(0);
       }
     }

  }  
  return 0;  
}