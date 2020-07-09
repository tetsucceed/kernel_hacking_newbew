#include <string.h>

/*asmlinkage*/ int mwrt(int a, char *l, int z);

int main(int argc, char **argv)
{
	char h[] = "Hello World\n";
	mwrt(1, h, strlen(h));

	return 0;
}
