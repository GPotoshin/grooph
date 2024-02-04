// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_draw.h: declaration of fns for drawing

#ifndef GROOPH_DRAW_H
#define GROOPH_DRAW_H

#include <stdlib.h>
#include "grooph_core.h"

int grFillBackground (GrImage *img, GrColor color);
int grPutDot (GrImage *img, int center[2], GrColor color, int width);
int grDrawLine (GrImage *img, int A[2], int B[2], GrColor color, int width);
int grDrawCircle (GrImage *img, int center[2], int radius, GrColor color, int width);
// pass NULL ro scheme. By convention verts should be in oreder
int grDrawPolygon (GrImage *img, const int *verts, const int n, GrColorScheme *scheme);

#endif /* GROOPH_DRAW_H */
