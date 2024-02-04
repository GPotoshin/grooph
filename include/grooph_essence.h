// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_essence.h: declaration of fns for working with structs in grooph_core.h 

#ifndef GROOPH_ESSENCE_H
#define GROOPH_ESSENCE_H

#include <stdio.h>
#include "grooph_core.h"

// Creates an image representation. Use grInitDefaultImage if you do not need finer control
GrImage *grInitImage (int width, int height, int *err);

// Inits a structure that uses libpng to represent a png file
GrPng *grInitPng (int width, int height, int bit_depth, int color_type,
        int interlace_method, int compression_method, int filter_method,
        int *err);

// Shortcut ro add png to image
void grAddDefaultPngToImage (GrImage *image, int *err);

// Universal object destroyer
void grDestroyObject (GrEmptyObject **obj);

// Writes image representation to a file
void grWriteImage (GrImage *img, FILE *fp);

// creater for GrColor structure
GrColor grInitColor (int r, int g, int b);

// Resets size and reallocates memory image representation
int grResetSize (GrImage *img, int width, int height);

#endif /* GROOPH_ESSENCE_H */
