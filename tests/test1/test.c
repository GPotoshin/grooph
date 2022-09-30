#include <stdlib.h>
#include "../include/grooph.h"

int main () {
	FILE *fp = fopen ("test1.png", "wb");
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
	grFullBackground (img, color);

	int p1[2] = {720, 50};
	int p2[2] = {1182, 850};
	color.red = 60;
	color.green = 56;
	color.blue = 24;
	
	grDrawLine (img, p1, p2, color, 3);
	p2[0] = 720;
	grDrawLine (img, p1, p2, color, 3);
	p2[0] = 258;
	grDrawLine (img, p1, p2, color, 3);
	p1[0] = 1182;
	p1[1] = 850;
	grDrawLine (img, p1, p2, color, 3);

	p1[0] = 720;
	p1[1] = 583;
	grDrawCircle (img, p1, 266, color, 3);
	grWriteImg (img, fp);
	grDestroyImg (img);
	fclose (fp);
	return 0;
}
