// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_essence.c: implimentation of fns for working with structs in grooph_core.h

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

#include <png.h>

#include "grooph_defs.h"
#include "grooph_core.h"
#include "grooph_essence.h"
#include "tools/grizmos.h"

GrImg *grInitImg (int filetype, int colortype,
			   int interlace, int compression, int filter, char *libver) {
	void *retval = NULL;


	GrImg *img = malloc (sizeof(GrImg));
	if (!img)
		goto _bailout;

	img->filetype = filetype;

	img->width = 0;
	img->height = 0;
	img->rows = NULL;

	if (filetype == GR_PNG) {
		img->colortype = colortype;
		img->interlace = interlace;
		img->compression = compression;
		img->filter = filter;

		img->partition = 2;

		img->libver = cstr(libver);
		if (!img->libver) {
			GR_LOG_ERROR("grInitImg", "can't copy libver");
			goto _bailout;
		}

		img->data = malloc (img->partition * sizeof(size_t));
		if (!img->data) {
			GR_LOG_ERROR("grInitImg", "can't alllocate data");
			goto _bailout;
		}

		img->data[0] = png_create_write_struct (img->libver, NULL, NULL, NULL);
		if(!img->data[0]) {
			GR_LOG_ERROR("grInitImg", "can't create data");
			goto _bailout;
		}

		img->data[1] = png_create_info_struct (img->data[0]);
		if (!img->data[1]) {
			GR_LOG_ERROR("grInitImg", "can't create info");
			goto _bailout;
		}
	}

	retval = img;
	return retval;

_bailout:
	grDestroyImg (img);
	return retval;
}


void grDestroyImg (GrImg *img) {
	if (img) {
		if (img->libver)
			free (img->libver);
		if (img->rows) {
			for (int i = 0; i < img->height; i++)
				if (img->rows[i])
					free (img->rows[i]);
			free (img->rows);
		}
		if (img->data) {
			for (int i = 0; i < img->partition; i++)
				if (img->data[i])
					free (img->data[i]);
			free (img->data);
		}

		free (img);
	}
}


void grWriteImg (GrImg *img, FILE *fp) {
	if (img->filetype == GR_PNG) {
		png_init_io (img->data[0], fp);
		png_set_IHDR (img->data[0], img->data[1], img->width, img->height, 8,
			img->colortype, img->interlace, img->compression, img->filter);
		png_write_info (img->data[0], img->data[1]);
		png_write_image (img->data[0], img->rows);
	}
}


GrImg *grInitDefaultImg (int filetype) {
	if (filetype == GR_PNG)
		return grInitImg (GR_PNG, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
						  PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE,
						  PNG_LIBPNG_VER_STRING);
	return NULL;
}


int grSetSize (GrImg *img, int width, int height)  {
	int retval = -1;
	size_t row_bytes = height * sizeof (GrBytep);
	size_t col_bytes = width * 3 * sizeof (GrByte);

	img->rows = malloc (row_bytes);
	if (!img->rows) {
		GR_LOG_ERROR("grSetSize", "can't allocate rows");
		goto _bailout;
	}

	bzero (img->rows, row_bytes);
	for (int i = 0; i < height; i++) {
		img->rows[i] = malloc (col_bytes);
		if (!img->rows[i]) {
			GR_LOG_ERROR("grSetSize", "can't allocate rows[i]");
			goto _bailout;
		}
	}


	img->width = width;
	img->height = height;

	retval = 0;
	return retval;
_bailout:
	img->width = 0;
	img->height = 0;

	if (img->rows) {
		for (int i = 0; i < height; i++)
			if (img->rows[i])
				free (img->rows[i]);
		free (img->rows);
	}

	return retval;
}


int grResetSize (GrImg *img, int width, int height) {
	int retval = -1;

// reserve copy
	size_t row_bytes = img->height * sizeof (GrBytep);
	size_t col_bytes = img->width * 3 * sizeof (GrByte);

	GrBytep *r_rows = NULL;

	*r_rows = malloc(row_bytes);
	if (!r_rows) {
		GR_LOG_ERROR("grResetSize", "can't allocate r_rows");
		goto _bailout;
	}

	bzero (r_rows, row_bytes);
	for (int i = 0; i < img->height; i++) {
		r_rows[i] = malloc(col_bytes);
		if (!r_rows[i]) {
			GR_LOG_ERROR("grResetSize", "can't allocate r_rows[i]");
			goto _bailout;
		}
		strncpy ((char *)r_rows[i], (const char *)img->rows[i], col_bytes);
	}

// trying to realloc
	row_bytes = height * sizeof (GrBytep);
	col_bytes = width * 3 * sizeof (GrByte);\

	void *p = img->rows;
	img->rows = realloc (img->rows, row_bytes);
	if (!img->rows) {
		img->rows = p;
		row_bytes = img->height * sizeof (GrBytep);
		goto _bailout;
	}

	bzero (img->rows, row_bytes);
	for (int i = 0; i < height; i++) {
		p = img->rows[i];
		img->rows[i] = realloc (img->rows[i], col_bytes);
		if (!img->rows[i]){
			GR_LOG_ERROR("grResetSize", "can't reallocate rows[i]");
			img->rows[i] = p;
			goto _bailout;
		}
	}

	img->height = height;
	img->width = width;

	retval = 0;
	return retval;

_bailout:
	for (int i = 0; i < row_bytes; i++) {
		if (img->rows[i])
			free (img->rows[i]);
	}
	free(img->rows);
	img->rows = r_rows;

	return retval;
}
