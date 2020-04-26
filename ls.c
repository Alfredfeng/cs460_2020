
#include "ucode.c"

#define BLK 1024
#define OWNER 000700
#define GROUP 000070
#define OTHER 000007

STAT utat, *sp;
int fd, n;
DIR *dp;
char f[32], cwdname[64], file[64];
char buf[1024];

DIR *dp;
char *cp;

void pdate(t) u8 t[4];
{
    printf("%c%c%c%c-%c%c-%c%c  ",
           (t[0] >> 4) + '0', (t[0] & 0x0F) + '0',
           (t[1] >> 4) + '0', (t[1] & 0x0F) + '0',
           (t[2] >> 4) + '0', (t[2] & 0x0F) + '0',
           (t[3] >> 4) + '0', (t[3] & 0x0F) + '0');
}

void ptime(t) u8 t[4];
{
    printf("%c%c:%c%c:%c%c  ",
           (t[0] >> 4) + '0', (t[0] & 0x0F) + '0',
           (t[1] >> 4) + '0', (t[1] & 0x0F) + '0',
           (t[2] >> 4) + '0', (t[2] & 0x0F) + '0');
}

void ls_file(STAT *sp, char *name, char *path)
{
    u16 mode;
    int mask, k, len;
    char fullname[32], linkname[60];

    mode = sp->st_mode;
    //printf("mode=%x ", mode);
    if ((mode & 0xF000) == 0x4000)
        mputc('d');

    if ((mode & 0xF000) == 0xA000)
        mputc('s');
    else if ((mode & 0xF000) == 0x8000)
        mputc('-');

    mask = 000400;
    for (k = 0; k < 3; k++)
    {
        if (mode & mask)
            mputc('r');
        else
            mputc('-');
        mask = mask >> 1;
        if (mode & mask)
            mputc('w');
        else
            mputc('-');
        mask = mask >> 1;
        if (mode & mask)
            mputc('x');
        else
            mputc('-');
        mask = mask >> 1;
    }

    if (sp->st_nlink < 10)
        printf("  %d ", sp->st_nlink);
    else
        printf(" %d ", sp->st_nlink);

    printf(" %d  %d", sp->st_uid, sp->st_gid);
    //align(sp->st_size);
    printf("%d ", sp->st_size);

    //    pdate(&sp->st_date); ptime(&sp->st_time);

    printf("%s", name);

    if ((mode & 0xF000) == 0xA000)
    {
        strcpy(fullname, path);
        strcat(fullname, "/");
        strcat(fullname, name);
        // symlink file: get its linked string
        len = readlink(fullname, linkname);
        printf(" -> %s", linkname);
    }

    printf("\n\r");
}

void ls_dir(STAT *sp, char *path)
{
    STAT dstat, *dsp;
    long size;
    char temp[32];
    int r;

    //printf("ls_dir %s\n", path); //getc();
    size = sp->st_size;
    fd = open(file, O_RDONLY); /* open dir file for READ */
    while ((n = read(fd, buf, 1024)))
    {
        cp = buf;
        dp = (DIR *)buf;

        while (cp < buf + 1024)
        {
            dsp = &dstat;
            strncpy(temp, dp->name, dp->name_len);
            temp[dp->name_len] = 0;
            f[0] = 0;
            strcpy(f, file);
            strcat(f, "/");
            strcat(f, temp);
            if (stat(f, dsp) >= 0)
                ls_file(dsp, temp, path);

            cp += dp->rec_len;
            dp = (DIR *)cp;
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    int r, i;
    sp = &utat;
    // for (i=0; i<argc; i++)
    //   printf("arg[%d]=%s ", i, argv[i]);
    printf("Now Running Jeff's ls\n\r");

    if (argc == 1)
    { /* for uls without any parameter ==> cwd */
        strcpy(file, "./");
    }
    else
    {
        strcpy(file, argv[1]);
    }

    if (stat(file, sp) < 0)
    {
        printf("cannot stat %s\n", argv[1]);
        exit(2);
    }

    if ((sp->st_mode & 0xF000) == 0x8000)
    {
        ls_file(sp, file, file);
    }
    else
    {
        if ((sp->st_mode & 0xF000) == 0x4000)
        {
            ls_dir(sp, file);
        }
    }

    exit(0);
}
