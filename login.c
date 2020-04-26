#include "ucode.c"

char buf[1024], uname[64], upass[64];
char pline[64], mytty[64], *pname[8];
char *cp, *cq, *cpp, *cqq;
int gid, uid, i, n;

// seems stdin,stdout,stderr are keywords in bcc
// int stdin, stdout, stderr; ==> auto_start symbol
int in, out, err;
int fd;
// login : call by init as "login tty0" | "login ttyS0" | ...
// Upon entry, name[0]=filename, name[1]=mytty string
char whattty[64];

int main(int argc, char *argv[])
{
  //Close all the io paths
  close(0); //close stdin
  close(1); //close stdout
  close(2); //close stderr

  strcpy(mytty, argv[1]);

  //reopen io for current terminal
  in = open(mytty, 0);
  out = open(mytty, 1);
  err = open(mytty, 1);

  //syscall magic
  fixtty(mytty); // register mytty string in PROC.tty[]
  gettty(whattty); //gets current tty name and stores in whattty
  printf("login: tty=%s\n", whattty);
  printf("LOGIN : open %s as stdin=%d, stdout=%d, stderr=%d\n",
         mytty, in, out, err);

  while (1)
  {
    //printf("------------------------------------\n\r");
    print2f("login:");
    gets(uname);
    print2f("password:");
    gets(upass);
    //printf("------------------------------------\n\r");

    /** open /etc/passwd file to get uname's uid, gid, HOME, program **/
    fd = open("/etc/passwd", 0);
    if (fd < 0)
    {
      printf("no passwd file\n");
      exit(1);
    }

    //reading from password file
    n = read(fd, buf, 2048);
    buf[n] = 0;

    cp = cq = buf; //tracking variables
    
    //processingg password file
    while (cp < &buf[n])
    {
      //change seperators
      while (*cq != '\n')
      {
        if (*cq == ' ')
          *cq = '-';
        if (*cq == ':')
          *cq = ' ';
        cq++;
      }

      //reset tracking for cq, then processing line again
      *cq = 0;
      //copy password line into pline
      strcpy(pline, cp);
      cpp = cqq = pline; 
      i = 0;

      //line round 2
      while (*cqq)
      {
        //seperating string at the spaces
        //and storing in array
        if (*cqq == ' ')
        {
          *cqq = 0;
          pname[i] = cpp;
          i++;
          cqq++;
          cpp = cqq;
          continue;
        }
        cqq++;
      }

      //if username and password match what is in file, login
      if (strcmp(uname, pname[0]) == 0 && strcmp(upass, pname[1]) == 0)
      {
        printf("LOGIN : Welcome! %s\n", uname);
        printf("LOGIN : cd to HOME=%s  ", pname[5]);
        chdir(pname[5]); // cd to pname[5] homedir of user

        gid = atoi(pname[2]); //group id, not used
        uid = atoi(pname[3]); 
        printf("change uid to %d\n", uid);
        chuid(uid, 0); // change gid, uid

        printf("exec to /bin/sh .....\n");

        //close the fd
        close(fd);
        exec("sh jksh start");
      }
      cq++;
      cp = cq;
    }
    printf("login failed, try again\n");
    close(fd);
  }
}
