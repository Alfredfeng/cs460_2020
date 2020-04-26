

#include "ucode.c"

char *cp, mytty[64];

char realname[64], linkname[64];
char newline = '\n';
char CR = '\r';

int main(int argc, char *argv[])
{
	int fd, i, n;
	char buf[1244], dummy;
	char *cp;

	print2f("Now Running Cat \n\r");

	fd = 0;
	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			printf("cat %s error\n", argv[1]);
			exit(0);
		}
	}

	if (argc < 2)
	{
		while (gets(buf))
		{
			print2f(buf);
			print2f("\n\r");
		}
		exit(0);
	}

	while ((n = read(fd, buf, 1024)))
	{
		buf[n] = 0;
		cp = buf;
		if (fd)
		{
			for (i = 0; i < n; i++)
			{
				write(1, &buf[i], 1);
				if (buf[i] == '\n')
					write(2, &CR, 1);
			}
		}

		else
		{ // fd=0 case
			cp = buf;
			if (*cp == '\r')
				write(2, &newline, 1);
			write(1, cp, 1);
		}
	}
	close(fd);
	exit(0);
}
