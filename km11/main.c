#include <stdio.h>

#define QUINS 4

int x[QUINS];
static int v;

void p();

void f(int xy)
{
	int iy, jx;

	if(xy == QUINS) {
		p();
		v++;
		return;
	}

	for(jx = 0; jx < QUINS; jx++) {
		for(iy = 0; iy < xy; iy++) {
			if((x[iy] - iy) == (jx - xy))
				break;
			if((x[iy] + iy) == (jx + xy))
				break;
			if(x[iy] == jx)
				break; 
		}

		if(iy == xy) {
			/*printf("in if jx %d iy %d xy %d\n", jx, iy, xy);*/
			x[xy] = jx;
			f(xy+1);
		}
	}
		
	return;
}

void p()
{
	int ix, y;
	int a = 0;

	while(a < QUINS) {
		printf("%d", x[a]);
		a++;
	}

	putchar('\n');

	for(ix = 0; ix < QUINS; ix++) {
		for(y = 0; y < QUINS; y++) {
			if(x[y] == ix) {
				printf(" 0 ");
				continue;
			}
			printf(" . ");
		}
		putchar('\n');
	}

	return;
}
	

int main(int argc, char **argv)
{
	int a = QUINS;

	f(0);
	printf("\n\n%d\n", v);

	return 0;
}
