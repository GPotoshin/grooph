/*
 * =====================================================================================
 *
 *       Filename:  GRDraw.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/14/2022 13:52:16
 *       Revision:  none
 *       Compiler:
 *
 *         Author:  George Potoshin (GP), george.potoshin@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

#include "GRDefs.h"
#include "GRTypes.h"
#include "GRDraw.h"
#include "GRizmos.h"

int GRFullBackground (GRImg *img, GRColor color) {
	int retval = -1;
	if (!img->rows)
		goto _bailout;

	retval = -2;
	if (img->filetype == GR_PNG) {
		for (int j = 0; j < img->height; j++) {
			if (!img->rows[j])
				goto _bailout;
			for (int i = 0; i < img->width; i++) {
				GRBytep bp = img->rows[j] + i*3;
				bp[0] = color.red;
				bp[1] = color.green;
				bp[2] = color.blue;
			}
		}
	}

	retval = 0;
_bailout:
	return retval;
}


int GRPutRainbowyDot (GRImg *img, GRInt2 p, GRColor color, int w) {
	int retval = -1;
	if (!img->rows)
		goto _bailout;

	retval = -2;
	if (img->filetype == GR_PNG) {
		for (int j = p.y - w; j <= p.y + w; j++) {
			if (!img->rows[j])
				goto _bailout;

			for (int i = p.x - w; i <= p.x + w; i++) {
				if (j < img->height && i < img->width && 
						((i-p.x)*(i-p.x) + (j-p.y)*(j-p.y) < w*w)) {
					GRBytep bp = img->rows[j] + i*3;
					bp[0] = (GRByte) fmax ((GRByte)(color.red * 1.5 * w /
							(1.5*w + hypot (i-p.x, j-p.y))),
							(GRByte)(bp[0] + color.red * 1.5 * w /
							(1.5*w + hypot (i-p.x, j-p.y))));
					bp[1] = (GRByte) fmax ((GRByte)(color.green * 1.5 * w /
							(1.5*w + hypot (i-p.x, j-p.y))),
							(GRByte)(bp[1] + color.green * 1.5 * w /
							(1.5*w + hypot (i-p.x, j-p.y))));
					bp[2] = (GRByte) fmax ((GRByte)(color.blue * 1.5 * w /
							(1.5*w + hypot (i-p.x, j-p.y))),
							(GRByte)(bp[2] + color.blue * 1.5 * w /
							(1.5*w + hypot (i-p.x, j-p.y))));
				}
			}
		}
	}

	retval = 0;
_bailout:
	return retval;
}


int GRDrawLine (GRImg *img, GRInt2 p1, GRInt2 p2, GRColor color, int w) {
	int retval = -1;
	GRInt2 p;
	if (p1.x == p2.x) {
		p.x = p1.x;
		for (p.y = fmin (p2.y, p1.y); p.y <= fmax (p2.y, p1.y); p.y += (w+1)/2) {
			GRPutRainbowyDot (img, p, color, w);
		}
	}
	if (p1.y == p2.y) {
		p.y = p1.y;
		for (p.x = fmin (p2.x, p1.x); p.x <= fmax (p2.x, p1.x); p.x += (w+1)/2) {
			GRPutRainbowyDot (img, p, color, w);
		}
	}

	if (p2.x < p1.x) {
		p1.y += p2.y;
		p2.y = p1.y - p2.y;
		p1.y -= p2.y;
		p1.x += p2.x;
		p2.x = p1.x - p2.x;
		p1.x -= p2.x;
	}

	p = p1;
	double k = (double)(p2.y - p1.y) / (p2.x - p1.x);
	double m = 0.5*(p1.y - k*p1.x + p2.y - k*p2.x);

	if (k*k < 1) {
		if (k > 0) {
			while (p.x <= p2.x) {
				GRPutRainbowyDot (img, p, color, w);
				p.x++;
				if (p.y < k*p.x + m) {
					p.y++;
				}
			}
		}
		else {
			while (p.x <= p2.x) {
				GRPutRainbowyDot (img, p, color, w);
				p.x++;
				if (p.y > k*p.x + m) {
					p.y--;
				}
			}
		}
	}
	else {
		if (k > 0) {
			while (p.y <= p2.y) {
				GRPutRainbowyDot (img, p, color, w);
				p.y++;
				if (p.x < (p.y - m) / k) {
					p.x++;
				}
			}
		}
		else {
			while (p.y >= p2.y) {
				GRPutRainbowyDot (img, p, color, w);
				p.y--;
				if (p.x < (p.y - m) / k) {
					p.x++;
				}
			}
		}
	}

	retval = 0;
_bailout:
	return retval;
}

int GRDrawCircle (GRImg *img, GRInt2 p, int r, GRColor color, int w) {
	GRInt2 v;
	double dt = 1.0 / r;
	for (double t = 0; t < 2 * M_PI; t += dt) {
		v.x = p.x + r*cos(t);
		v.y = p.y + r*sin(t);
		GRPutRainbowyDot (img, v, color, w);
	}
	return 0;
}
