// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_core.h: basic structures with img data


#ifndef GROOPH_CORE_H
#define GROOPH_CORE_H

const char * grooph_version = "0.0.2";

enum grooph_file_type {
    GR_PNG,
    GR_JPG,
};

typedef unsigned char GrByte;
typedef unsigned char * GrBytep;

typedef struct grooph_img {
	void *pNext;

	int filetype;
	int colortype;
	int interlace;
	int compression;
	int filter;

	char *libver;

	int height;
	int width;

	GrBytep *rows;

	void **data;
	int partition;
} GrImg;

typedef struct {
	GrByte red;
	GrByte green;
	GrByte blue;
} GrColor;

typedef struct {
} GrColorScheme;

#endif /* GROOPH_CORE_H */
