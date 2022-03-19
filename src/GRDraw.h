/*
 * =====================================================================================
 *
 *       Filename:  GRDraw.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/06/2022 20:37:14
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  George Potoshin (GP), george.potoshin@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GRDRAW_H
#define GRDRAW_H

#include "GRTypes.h"

int GRFullBackground (GRImg *img, GRColor color); 
int GRPutRainbowyDot (GRImg *img, GRInt2 p, GRColor color, int w);
int GRDrawLine (GRImg *img, GRInt2 p1, GRInt2 p2, GRColor color, int w);
int GRDrawCircle (GRImg *img, GRInt2 p, int r, GRColor color, int w);

#endif /* GRDRAW_H */
