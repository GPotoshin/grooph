v0.0.4
Simple library for rendering/drawing pictures
 
## Goals
Create a simple pure C interface for drawing pictures. The idea came to me as I was preparing homework for my school IT classes.
 
## Compiling
1) install ```pnglib``` on your machine
2) run ```make```

## Use
```c
#include <stdio.h>
#include "../include/grooph.h"

int main () {
	int *err = 0;
	FILE *fp = fopen ("test1.png", "wb");
	if (!fp) {
		printf ("Can't open file!\n");
		return 0;
	}

	GrImage *img = grInitImage(1440, 900, NULL);
	if (!img) {
		printf ("Can't init image\n");
		goto _bailout;
	}
	grAddDefaultPngToImage(img, err);
	if (err != GR_SUCCESS) {
		goto _bailout;
	}

	GrColor color = {13, 13, 13};
	grFillBackground (img, color);

	int p1[2] = {720, 50};
	int p2[2] = {1182, 850};
	color.red = 60;
	color.green = 56;
	color.blue = 24;

	puts ("line 1: from {720, 50} to {1182, 850}");
	grDrawLine (img, p1, p2, color, 3);
	p2[0] = 720;
	puts ("line 2: from {720, 50} to {720, 850}");
	grDrawLine (img, p1, p2, color, 3);
	p2[0] = 258;
	puts ("line 3: from {720, 50} to {258, 850}");
	grDrawLine (img, p1, p2, color, 3);
	p1[0] = 1182;
	p1[1] = 850;
	puts ("line 4: from {1182, 850} to {258, 850}");
	grDrawLine (img, p1, p2, color, 3);

	p1[0] = 720;
	p1[1] = 583;
	puts ("circle: center {720, 583} and radius 266");
	grDrawCircle (img, p1, 266, color, 3);
	grWriteImage(img, fp);

_bailout:
	if (img) {
		grDestroyObject ((GrEmptyObject **)&img);
	}
	if (fp) {
		fclose (fp);
	}
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
