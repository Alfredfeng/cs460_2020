
#include "ucode.c"

int main(int argc, char *argv[])
{
  int fd, gd, n, c;
  char buf[1024], *cp;

  print2f("Entering my l2u\n");
  printf("\nargc = %d  argv = ", argc);

  for (n = 0; n < argc; n++)
    printf("%s ", argv[n]);
  printf("\n");

  if (argc == 1)
  { // stdin to stdout
    while ((c = getc()) != EOF)
    {
      //confirms c is a letter
      c &= 0x7F;
      if (c >= 'a' && c <= 'z')
        mputc(c - 'a' + 'A');
      else
        mputc(c);

      if (c == '\r')
        mputc('\n');
    }
    exit(0);
  }
  if (argc < 3)
  {
    printf("usage : l2u f1 f2\n");
    exit(1);
  }
  fd = open(argv[1], O_RDONLY);
  if (fd < 0)
  {
    exit(1);
  }
  gd = open(argv[2], O_WRONLY | O_CREAT);

  while (n = read(fd, buf, 1024))
  {
    cp = buf;
    while (cp < buf + n)
    {
      if (*cp >= 'a' && *cp <= 'z')
      {
        *cp = *cp - 'a' + 'A';
      }
      cp++;
    }
    write(gd, buf, n);
  }
  printf("done\n");
}
