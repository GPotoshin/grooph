// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_essence.h: declaration of fns for working with structs in grooph_core.h 

#ifndef GROOPH_ESSENCE_H
#define GROOPH_ESSENCE_H

#include "grooph_core.h"

GrImg *grInitImg (int filetype, int colortype, int intarelace, int compression, int filter, char *libver);
void grDestroyImg (GrImg *img);
void grWriteImg (GrImg *img, FILE *fp);

GrImg *grInitDefaultImg (int filetype);

int grSetSize (GrImg *img, int width, int height);
int grResetSize (GrImg *img, int width, int height);

#endif /* GROOPH_ESSENCE_H */
