
#include "ucode.c"
#define printk printf

int console, s0, s1;

void login()
{
  //QEMU terminal
  exec("login /dev/tty0"); 
}
void s0login()
{
  //first serial tereminal -> first non-QEMU terminal access
  exec("login /dev/ttyS0"); 
}
void s1login()
{
  //second serial tereminal -> second non-QEMU terminal access
  exec("login /dev/ttyS1"); 
}

void parent()
{
  int pid, status;
  printf("INIT : waiting ...\n");

  while (1)
  {
    //wait the process
    pid = wait(&status);

    if (pid == console)
    {
      //fork a new console
      console = fork();
      //if sh at console dies, run login again for console
      if (console)
        continue;
      else
        login();
    }
    if (pid == s0)
    {
      //fork new for s0
      s0 = fork();
      //if sh at s0 dies, run login for s0
      if (s0)
        continue;
      else
        s0login();
    }
    if (pid == s1)
    {
      //fork new  terminal  for s1
      s1 = fork();
      //if sh at s1 dies, run login again for s1
      if (s1)
        continue;
      else
        s1login();
    }
    //when a process exits, bury it
    printf("INIT: buried an orphan child task %d\n", pid);
  }
}

int main(int argc, char *argv[])
{
  int in, out;
  int pid;

  // open read/write in QEMU console
  in = open("/dev/tty0", O_RDONLY);
  out = open("/dev/tty0", O_WRONLY);

  printf("proc %d in Umode ", getpid());
  printf("argc=%d %s %s\n", argc, argv[0], argv[1]);

  printf("%s\n", "INIT: fork a login task on console");

  //fork a console instance
  console = fork();
  //in parrent;
  if (console)
  { // parent
    //fork login for other terminals
    printf("fork login on ttyS0\n");
    s0 = fork();
    if (s0)
    {
      s1 = fork();
      if (s1)
        parent();
      else
        s1login();
    }
    else
      s0login();
  }
  //if youre not in parent, login
  else
    login();
}
