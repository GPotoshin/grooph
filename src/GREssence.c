/*
 * =====================================================================================
 *
 *       Filename:  GREssence.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/14/2022 13:52:16
 *       Revision:  none
 *       Compiler:
 *
 *         Author:  George Potoshin (GP), george.potoshin@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

#include <png.h>

#include "GRDefs.h"
#include "GRTypes.h"
#include "GREssence.h"
#include "GRizmos.h"

GRImg *GRInitImg (int filetype, int colortype,
			   int interlace, int compression, int filter, char *libver) {
	void *retval = 0;


	GRImg *img = malloc(sizeof(GRImg));
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
		if (!img->libver)
			goto _bailout;

		img->data = malloc (img->partition * sizeof(size_t));
		if (!img->data)
			goto _bailout;

		img->data[0] = png_create_write_struct (img->libver, NULL, NULL, NULL);
		if(!img->data[0])
			goto _bailout;

		img->data[1] = png_create_info_struct (img->data[0]);
		if (!img->data[1])
			goto _bailout;
	}

	retval = img;
	return retval;

_bailout:
	GRDestroyImg (img);
	return retval;
}


void GRDestroyImg (GRImg *img) {
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


void GRWriteImg (GRImg *img, FILE *filep) {
	if (img->filetype == GR_PNG) {
		png_init_io (img->data[0], filep);
		png_set_IHDR (img->data[0], img->data[1], img->width, img->height, 8,
			img->colortype, img->interlace, img->compression, img->filter);
		png_write_info (img->data[0], img->data[1]);
		png_write_image (img->data[0], img->rows);
	}
}


GRImg *GRInitDefaultImg (int filetype) {
	if (filetype == GR_PNG)
		return GRInitImg (GR_PNG, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
						  PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE,
						  PNG_LIBPNG_VER_STRING);
	return NULL;
}


int GRSetSize (GRImg *img, int width, int height)  {
	int retval = -1;
	size_t row_bytes = height * sizeof (GRBytep);
	size_t col_bytes = width * 3 * sizeof (GRByte);

	img->rows = malloc (row_bytes);
	if (!img->rows)
		goto _bailout;

	bzero (img->rows, row_bytes);
	for (int i = 0; i < height; i++) {
		img->rows[i] = malloc (col_bytes);
		if (!img->rows[i])
			goto _bailout;
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


int GRResetSize (GRImg *img, int width, int height) {
	int retval = -1;

// reserve copy
	size_t row_bytes = img->height * sizeof (GRBytep);
	size_t col_bytes = img->width * 3 * sizeof (GRByte);

	GRBytep *r_rows = NULL;

	*r_rows = malloc(row_bytes);
	if (!r_rows)
		goto _bailout;

	bzero (r_rows, row_bytes);
	for (int i = 0; i < img->height; i++) {
		r_rows[i] = malloc(col_bytes);
		if (!r_rows[i])
			goto _bailout;
		strncpy ((char *)r_rows[i], (const char *)img->rows[i], col_bytes);
	}

// trying to realloc
	row_bytes = height * sizeof (GRBytep);
	col_bytes = width * 3 * sizeof (GRByte);\

	void *p = img->rows;
	img->rows = realloc (img->rows, row_bytes);
	if (!img->rows) {
		img->rows = p;
		row_bytes = img->height * sizeof (GRBytep);
		goto _bailout;
	}

	bzero (img->rows, row_bytes);
	for (int i = 0; i < height; i++) {
		p = img->rows[i];
		img->rows[i] = realloc (img->rows[i], col_bytes);
		if (!img->rows[i]){
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
