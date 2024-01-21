#include <stdlib.h>
#include "../include/grooph.h"

int main () {
	FILE *fp = fopen ("test2.png", "wb");
	if (!fp) {
		printf ("Can't open file!\n");
		return 0;
	}

	GrImg *img = grInitDefaultImg (GR_PNG);
	if (!img) {
		printf ("Can't init image\n");
		return 0;
	}

	if (grSetSize (img, 1440, 900)) {
		printf ("grSetSize: Can't allocate memory!\n");
		return 0;
	}

	GrColor color = {13, 13, 13};
	grFillBackground (img, color);

	int **p = (int* []){(int []){720, 50}, (int []){800, 120}, (int []){1000, 120},
		(int []){900, 200}, (int []){920, 280}, (int []){720, 240},
		(int []){520, 280}, (int []){540, 200}, (int []){440, 120},
		(int []){640, 120}};

	grDrawPolygon (img, (const int **)p, 10, NULL);
	grWriteImg (img, fp);
	grDestroyImg (img);
	fclose (fp);
	return 0;
}
