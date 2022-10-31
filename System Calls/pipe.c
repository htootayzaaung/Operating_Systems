//C-program to illustrate pipe system call in C

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MSGSIZE 15

char *msg1 = "Hello World #1";
char *msg2 = "Hello World #2";
char *msg3 = "Hello World #3";

int main()
{
  char inbuf[MSGSIZE];
  int p[2];

  if (pipe(p) < 0)              //Returns 0 on success and -1 on error
  {
    exit(1);
  }

  write(p[1], msg1, MSGSIZE);
  write(p[1], msg2, MSGSIZE);
  write(p[1], msg3, MSGSIZE);

  for (int i = 0; i < 3; i++)
  {
    read(p[0], inbuf, MSGSIZE);
    printf("%s\n", inbuf);
  }
  return 0;
}
