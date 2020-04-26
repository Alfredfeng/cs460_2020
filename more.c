
#include "ucode.c"

char lines, buf[1024];
char uline[2048];

char mytty[64];
char w;
char CR = '\r';
char newline = '\n';
char c;

main(int argc, char *argv[])
{
   int fd, gd, n, i, r;

   lines = 0;
   gettty(mytty);
   // printf("tty = %s\n", mytty);
   gd = open(mytty, 0);
   //printf("gd=%d\n", gd);

   print2f("Now Entering my more\n\r");

   fd = 0; /* default infile = stdin */
   if (argv[1])
   {
      fd = open(argv[1], 0); /* open input file for READ */
      if (fd < 0)
         exit(1);
   }

   if (fd)
   {
      n = read(fd, buf, 1024);

      while (n > 0)
      {
         for (i = 0; i < n; i++)
         {
            c = buf[i];
            write(1, &c, 1);
            if (c == '\n')
            {
               lines++;
               write(2, &CR, 1);
            }
            if (lines > 25)
            {
               read(gd, &w, 1);
               if (w == '\r' || w == '\n')
                  lines--;
               if (w == ' ')
                  lines = 0;
            }
         }
         n = read(fd, buf, 1024);
      }
   }
   else
   {
      while (getline(uline))
      { // getline() does not show input chars
         printf("%s\r", uline);
         lines++;
         if (lines > 25)
         {
            n = read(gd, &w, 1);
            if (w == '\r' || w == '\n')
               lines--;
            if (w == ' ')
               lines = 0;
         }
      }
   }
}
