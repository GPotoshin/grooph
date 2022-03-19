/*
 * =====================================================================================
 *
 *       Filename:  GREssence.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/06/2022 20:39:21
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  George Potoshin (GP), george.potoshin@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#ifndef GRESSENCE_H
#define GRESSENCE_H

#include "GRTypes.h"

GRImg *GRInitImg (int filetype, int colortype, int intarelace,
				int compression, int filter, char *libver);
void GRDestroyImg (GRImg *img);
void GRWriteImg (GRImg *img, FILE *filep);

GRImg *GRInitDefaultImg (int filetype);

int GRSetSize (GRImg *img, int width, int height);
int GRResetSize (GRImg *img, int width, int height);

#endif /* GRESSENCE_H */
