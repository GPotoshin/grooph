// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_draw.h: declaration of fns for drawing

#ifndef GROOPH_DRAW_H
#define GROOPH_DRAW_H

#include <stdlib.h>
#include "grooph_core.h"

int grFillBackground (GrImg *img, GrColor color);
int grPutDot (GrImg *img, int center[2], GrColor color, uint32_t width);
int grDrawLine (GrImg *img, int A[2], int B[2], GrColor color, uint32_t width);
int grDrawCircle (GrImg *img, int center[2], uint32_t radius, GrColor color, uint32_t width);
// scheme is not working yet ... so pass there NULL
int grDrawPolygon (GrImg *img, const int **p, const int n, GrColorScheme *scheme);

#endif /* GROOPH_DRAW_H */
