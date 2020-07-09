#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH 4096

void rec_ls(char *path);

int main(int argc, char *argv[])
{
	char p[MAX_PATH];

	strcpy(p, getenv("PWD"));
	rec_ls(p);

	return 0;
}

void rec_ls(char *path)
{
        char tbuf[255];
	char path2[MAX_PATH];
        int len;
        DIR *ldir;
        struct dirent *d_point;
        struct stat inf;

        ldir = opendir(path);

        while((d_point = readdir(ldir)) != NULL) {
		sprintf(path2, "%s/%s", path, d_point->d_name);
                lstat(path2, &inf);

		if(!strcmp(d_point->d_name, "."))
			continue;
		if(!strcmp(d_point->d_name, ".."))
			continue;

		if(S_ISDIR(inf.st_mode)) {
			printf("%s\n", path2);
			rec_ls(path2);
		}

                ctime_r(&inf.st_mtime, tbuf);
                len = strlen(tbuf);
                tbuf[len-1] = ' ';
                printf("%o %d %d %d %s %s\n", inf.st_mode, inf.st_uid,
                        inf.st_gid, inf.st_size, tbuf, d_point->d_name);
        }

        closedir(ldir);

	return;
}

