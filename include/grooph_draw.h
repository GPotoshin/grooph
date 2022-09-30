// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_draw.h: declaration of fns for drawing

#ifndef GROOPH_DRAW_H
#define GROOPH_DRAW_H

#include "grooph_core.h"

int grFullBackground (GrImg *img, GrColor color);
int grPutDot (GrImg *img, int p[2], GrColor color, int w);
int grDrawLine (GrImg *img, int p1[2], int p2[2], GrColor color, int w);
int grDrawCircle (GrImg *img, int p[2], int r, GrColor color, int w);
// scheme is not working yet ...
int grDrawPolygon (GrImg *img, const int **p, const int n, GrColorScheme *scheme);

#endif /* GROOPH_DRAW_H */
