// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_essence.h: declaration of fns for working with structs in grooph_core.h 

#ifndef GROOPH_ESSENCE_H
#define GROOPH_ESSENCE_H

#include <stdio.h>
#include "grooph_core.h"

// Creates an image representation. Use grInitDefaultImg if you do not need finer control
GrImg *grInitImg (int filetype, int colortype, int intarelace, int compression, int filter, char *libver);

// Destroys image representation
void grDestroyImg (GrImg *img);

// Writes image representation to a file
void grWriteImg (GrImg *img, FILE *fp);

// Creates default image for type passed
GrImg *grInitDefaultImg (int filetype);

// creater for GrColor structure
GrColor grInitColor (int r, int g, int b);

// Sets size and allocates memory for image representation
int grSetSize (GrImg *img, int width, int height);

// Resets size and reallocates memory image representation
int grResetSize (GrImg *img, int width, int height);

#endif /* GROOPH_ESSENCE_H */
