/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/04/2022 11:44:26
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  George Potoshin (GP), george.potoshin@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "include/grooph.h"

int main () {
	FILE *fp = fopen ("test.png", "wb");
	if (!fp) {
		printf ("Can't open file!\n");
		return 0;
	}

	GRImg *img = GRInitDefaultImg (GR_PNG);
	if (!img) {
		printf ("Can't init image\n");
		return 0;
	}

	if (GRSetSize (img, 1440, 900)) {
		printf ("GRSetSize: Can't allocate memory!\n");
		return 0;
	}

	GRColor color = {13, 13, 13};
	GRFullBackground (img, color);

	GRInt2 p1 = {720, 50};
	GRInt2 p2 = {1182, 850};
	color.red = 16;
	color.green = 12;
	color.blue = 24;
	
	GRDrawLine (img, p1, p2, color, 3);
	p2.x = 720;
	GRDrawLine (img, p1, p2, color, 3);
	p2.x = 266;
	GRDrawLine (img, p1, p2, color, 3);
	p1.x = 1182;
	p1.y = 850;
	GRDrawLine (img, p1, p2, color, 3);

	p1.x = 720;
	p1.y = 583;
	GRDrawCircle (img, p1, 266, color, 3);
	GRWriteImg (img, fp);
	fclose (fp);
	return 0;
}
