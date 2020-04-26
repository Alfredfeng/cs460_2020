
/********* cp.c file ***************/

#include "ucode.c"

//void putchar(const char c){ }

int fd, gd, n, count;
char buf[1024], dummy = 0;

int main(int argc, char *argv[])
{
  print2f("Entering my cp\n\r");

  if (argc < 3)
  {
    printf("Usage: cp src dest\n");
    exit(1);
  }
  fd = open(argv[1], O_RDONLY);
  if (fd < 0)
  {
    printf("open src %s error\n", argv[1]);
    exit(2);
  }

  gd = open(argv[2], O_WRONLY | O_CREAT);
  if (gd < 0)
  {
    printf("open dest %s error\n", argv[2]);
    exit(2);
  }
  dummy = 0;
  count = 0;
  while ((n = read(fd, buf, 1024)))
  {
    printf("n=%d ", n);
    write(gd, buf, n);
    count += n;
  }
  printf("%d bytes copied\n", count);
}
