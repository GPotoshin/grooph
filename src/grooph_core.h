// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_core.h: basic structures with img data


#ifndef GROOPH_CORE_H
#define GROOPH_CORE_H

#include <png.h>

enum gr_result {
    GR_SUCCESS,
    GR_OUT_OF_MEMORY_ERROR,
    GR_NULL_POINTER_ERROR,
};

enum grooph_file_type {
    GR_PNG,
    GR_JPG,
};

typedef unsigned char GrByte;
typedef unsigned char * GrBytep;

enum grooph_structure_type {
    GR_IMAGE_STRUCTURE,
    GR_PNG_STRUCTURE,
};

// Uses as down cast to do polymorphism. Used to minimise exposed surface
typedef struct empty_object {
    void *pNext;
    int type;
} GrEmptyObject;

// Contains only rows and is complete enough to just work with binary data
typedef struct grooph_img {
	void *pNext;
    int type;   

	int height;
	int width;

	GrBytep *rows;
} GrImage;

// add it to GrImage->pNext to be able to write binary data to png file
typedef struct grooph_png {
    void *pNext; 
    int type;

    png_structp png_ptr;
    png_infop info_ptr;
} GrPng;

typedef struct {
	GrByte red;
	GrByte green;
	GrByte blue;
} GrColor;

typedef struct {
} GrColorScheme;

#endif /* GROOPH_CORE_H */
