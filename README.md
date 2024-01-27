v0.0.3
Simple library for rendering/drawing pictures
 
## Goals
Create a simple pure C interface for drawing pictures. The idea came to me as I was preparing homework for my school IT classes.
 
## Compiling
1) install ```pnglib``` on your machine
2) run ```make```

## Use
```c
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
	grFillBackground (img, color);

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
```
for more examples take a look at ```tests``` folder

## Tests
to compile:
1) compile grooph
2) ```make include```
3) ```sh test_prepare.sh```
4) ```cd tests```
5) ```make```

Executable files ```t``` are in ```test1```, ```test2``` 

## To Do
* Add support for obj files
* Add 3d graphics functions
* Add math plots
* Test and prepare for Windows
* Add GPU support
