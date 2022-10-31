#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
  printf("Hello World (pid: %d)\n", (int) getpid());

  int rc = fork();

  if (rc < 0)                     //fork failed
  {
      fprintf(stderr, "fork failed\n");
      exit(1);
  }

  else if (rc == 0)               //child new process
  {
    printf("Hello, I am child (pid: %d)\n", (int) getpid());
  }

  else
  {
    printf("Hello, I am a parent of %d (pid:%d)\n", rc, (int) getpid());
  }
  
  return 0;
}
