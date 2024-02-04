// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_essence.c: implimentation of fns for working with structs in grooph_core.h

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include <png.h>

#include "../deps/slog/slog.h"
#include "grooph_core.h"
#include "grooph_essence.h"

void grDestroyObject (GrEmptyObject **obj) {
	if (obj == NULL) {
		return;
	}

	if (*obj == NULL) {
		return;
	}

	switch ((*obj)->type) {
		case GR_IMAGE_STRUCTURE:
			;GrImage *img = (GrImage *) *obj;
			if (img->rows) {
				for (int i = 0; i < img->height; i++) {
					if (img->rows[i]) {
						free(img->rows[i]);
					}
				}
				free(img->rows);
			}
			break;

		case GR_PNG_STRUCTURE:
			;GrPng *png = (GrPng *) *obj;
			png_destroy_write_struct(&png->png_ptr, &png->info_ptr);
			break;
	}
	grDestroyObject((GrEmptyObject**)&(*obj)->pNext);
	free (*obj);
	*obj = NULL;
}


GrImage *grInitImage (int width, int height, int *err) {
	GrImage *retval = malloc (sizeof(GrImage));
	if (!retval) {
		SLOG_ERROR("grInitImage", "can't allocate memory for GrImage");
		if (err) {
			*err = GR_OUT_OF_MEMORY_ERROR;
		}
		goto _bailout;
	}

	retval->pNext = NULL;
	retval->type = GR_IMAGE_STRUCTURE;

	retval->rows = calloc (height, sizeof(GrBytep));
	if (!retval->rows) {
		SLOG_ERROR("grInitImage", "can't allocate memory for rows");
		if (err) {
			*err = GR_OUT_OF_MEMORY_ERROR;
		}
		goto _bailout;
	}
	for (int i = 0; i < height; i++) {
		retval->rows[i] = calloc (width*4, sizeof(GrByte));
		if (!retval->rows[i]) {
			SLOG_ERROR("grInitImage", "can't allocate memory for a row");
			if (err) {
				*err = GR_OUT_OF_MEMORY_ERROR;
			}
			goto _bailout;
		}
	}

	retval->height = height;
	retval->width = width;

	return retval;
_bailout:
	grDestroyObject ((GrEmptyObject **) &retval);
	return NULL;
}


GrPng *grInitPng (int width, int height, int bit_depth, int color_type,
		int interlace_method, int compression_method, int filter_method,
		int *err) {
	GrPng *retval = malloc (sizeof(GrPng));

	retval->pNext = NULL;
	retval->type = GR_PNG_STRUCTURE;

	if (!retval) {
		SLOG_ERROR("grInitPng", "can't allocate memory for GrPng");
		if (err) {
			*err = GR_OUT_OF_MEMORY_ERROR;
		}
		goto _bailout;
	}

	retval->png_ptr = png_create_write_struct (
			PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!retval->png_ptr) {
		SLOG_ERROR("grInitPng", "can't allocate memory for png_structure");
		if (err) {
			*err = GR_OUT_OF_MEMORY_ERROR;
		}
		goto _bailout;
	}

	retval->info_ptr = png_create_info_struct (retval->png_ptr);
	if (!retval->info_ptr) {
		SLOG_ERROR("grInitPng", "can't create memory for png_info");
		if (err) {
			*err = GR_OUT_OF_MEMORY_ERROR;
		}
		goto _bailout;
	}

	png_set_IHDR (retval->png_ptr, retval->info_ptr, width, height, bit_depth,
			color_type, interlace_method, compression_method, filter_method);

	return retval;
_bailout:
	grDestroyObject ((GrEmptyObject **)&retval);

	return NULL;
}

void grAddDefaultPngToImage (GrImage *image, int *err) {
	if (!image) {
		SLOG_ERROR("grAddDefaultPngToImage", "pointer to image is NULL");
		if (err) {
			*err = GR_NULL_POINTER_ERROR;
		}
		return ;
	}
	image->pNext = grInitPng(image->width, image->height, 8,
			PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
			PNG_FILTER_TYPE_BASE, err);
}

void grWriteImage (GrImage *img, FILE *fp) {
	if (!img->pNext) {
		SLOG_WARNING("grWriteImage",
			"You did not specify output format, the corisponding structure\n"
			"should be passed to pNext. For example: imgp->pNext = pngp");
		return;
	}

	GrEmptyObject *obj = img->pNext;

	switch (obj->type) {
		case GR_PNG_STRUCTURE:
			;GrPng *png = (void *)obj;
			png_init_io (png->png_ptr, fp);
			png_write_info (png->png_ptr, png->info_ptr);
			png_write_image (png->png_ptr, img->rows);
			break;
		default:
			SLOG_WARNING("grWriteImage", "bad file type from image");
	}
}

GrColor grInitColor (int r, int g, int b) {
	GrColor color = {r, g, b};
	return color;
}

int grResetSize (GrImage *img, int width, int height) {
	int retval = -1;

// reserve copy
	size_t row_bytes = img->height * sizeof (GrBytep);
	size_t col_bytes = img->width * 4 * sizeof (GrByte);

	GrBytep *r_rows = NULL;

	*r_rows = malloc(row_bytes);
	if (!r_rows) {
		SLOG_ERROR("grResetSize", "can't allocate r_rows");
		goto _bailout;
	}

	bzero (r_rows, row_bytes);
	for (int i = 0; i < img->height; i++) {
		r_rows[i] = malloc(col_bytes);
		if (!r_rows[i]) {
			SLOG_ERROR("grResetSize", "can't allocate r_rows[i]");
			goto _bailout;
		}
		memcpy ((char *)r_rows[i], (const char *)img->rows[i], col_bytes);
	}

// trying to realloc
	row_bytes = height * sizeof (GrBytep);
	col_bytes = width * 4 * sizeof (GrByte);\

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
		if (!img->rows[i]) {
			SLOG_ERROR("grResetSize", "can't reallocate rows[i]");
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
		if (img->rows[i]) {
			free (img->rows[i]);
		}
	}
	free(img->rows);
	img->rows = r_rows;

	return retval;
}
