#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>
#include <fstab.h>

#define BUFSZ 1024
#define STDLEN 100
#define NLINES 20

#define STR_EXT3 "ext3"
#define STR_EXT2 "ext2"
#define STR_REISERFS "reiserfs"
#define STR_XFS "xfs"

struct device_l {
	char dev_id[STDLEN];
	char mntpt[STDLEN];
	char fstype[NLINES];
	unsigned long bsize;
	unsigned long bfree;
	struct device_l *next;
};

struct device_l *ddev;
	

int main(int argc, char *argv[])
{
	struct statfs fst;
	struct fstab *ftab;
	int maj, min, ret;
	unsigned long long bks;
	char dev_name[BUFSZ], str[BUFSZ], *s;
	FILE *fptr;

	ret = 0;

	if(argc < 2) {
		printf("Usage: %s [MOUNTPOINT]\n", argv[0]);
		return 0;
	}

	ret = statfs(argv[1], &fst);
	if(ret != 0) {
		perror("statfs");
		return -1;
	}

	printf("Availble: %lu, Free: %lu, Total: %llu\n", fst.f_bsize,
			fst.f_bfree, fst.f_blocks);
	while((ftab = getfsent()) != NULL)
		printf("%s %s %s %s\n", ftab->fs_spec, ftab->fs_file, ftab->fs_vfstype,
			ftab->fs_mntops);
	return 0;
}

int getDeviceList(struct device_l *d)
{
	struct fstab *ftab;
	struct statfs fst;

	if(d == NULL) {
		d = malloc(sizeof(struct device_l));
		if(d == NULL)
			return -1;
	}

	while((ftab = getfsent()) != NULL) {
		if(!strcmp(ftab->fs_vfstype, STR_EXT3)) {
			
