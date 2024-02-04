// This is an open source non-commercial project.
// Author: George Potoshin <george.potoshin@gmail.com>
// grooph_draw.c: implementation of fns for drawing

#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "../deps/slog/slog.h"
#include "grooph_core.h"
#include "grooph_draw.h"

// a hack to do sorting depending on state that cannot be passed to compare fn
static const int * array_copy;

int grFillBackground (GrImage *img, GrColor color) {
	int retval = -1;
	if (!img->rows) {
		SLOG_ERROR("grFillBackground", "image representation does not have allocated memory");
		goto _bailout;
	}

	retval = -2;
	for (int j = 0; j < img->height; j++) {
		if (!img->rows[j]) {
			SLOG_ERROR("grFillBackground", "not all rows are allocated");
			goto _bailout;
		}
		for (int i = 0; i < img->width; i++) {
			GrBytep bp = img->rows[j] + i*3;
			bp[0] = color.red;
			bp[1] = color.green;
			bp[2] = color.blue;
		}
	}

	retval = 0;
_bailout:
	return retval;
}


int grPutDot (GrImage *img, int center[2], GrColor color, int radius) {
	int retval = -1;
	if (!img->rows) {
		SLOG_ERROR("grPutDot", "image representation does not have allocated memory");
		goto _bailout;
	};

	retval = -2;
	int top = fmax (0, center[1] - radius);
	int bottom = fmin (img->height, center[1] + radius);
	int left = fmax (0, center[0] - radius);
	int right = fmin (img->width, center[0] + radius);
	for (int j = top; j <= bottom; j++) {
		if (!img->rows[j]) {
			SLOG_ERROR("grPutDot", "not all rows are allocated");
			goto _bailout;
		}
		for (int i = left; i <= right; i++) {
			if (((i-center[0])*(i-center[0]) + (j-center[1])*(j-center[1])
						< radius*radius)) {
				GrBytep bp = img->rows[j] + i*3;
				bp[0] = color.red;
				bp[1] = color.green;
				bp[2] = color.blue;
			}
		}
	}

	retval = 0;
_bailout:
	return retval;
}

int grDrawLine (GrImage *img, int a[2], int b[2], GrColor color, int width) {
	// printf ("(%d, %d)----(%d, %d)\n", a[0], a[1], b[0], b[1]);
	// equation is k1*x + k2*y = const
	if (a[0] == b[0]) {
		int left = fmax (0, a[0]-width);
		int right = fmin (img->width-1, a[0]+width);
		int top = fmin (a[1], b[1]);
		int bottom = fmax (a[1], b[1]);

		for (int i = left; i <= right; i++) {
			for (int j = top; j <= bottom; j++) {
				GrBytep bp = img->rows[j] + 3*i;
				*(GrColor *)bp = color; // polymorphism in c
			}
		}
	} else if (a[1] == b[1]) {
		int left = fmin (a[0], b[0]);
		int right = fmax (a[0], b[0]);
		int top = fmax (0, b[1]-width);
		int bottom = fmin (img->height-1, b[1]+width);

		for (int i = left; i <= right; i++) {
			for (int j = top; j <= bottom; j++) {
				GrBytep bp = img->rows[j] + 3*i;
				*(GrColor *)bp = color; // polymorphism in c
			}
		}
	} else {
		if (b[0] < a[0]) {
			int *c = a;
			a = b;
			b = c;
		}
		int c1 = a[1]*(b[0] - a[0]) - a[0]*(b[1] - a[1]);
		double norm = hypot(b[0]-a[0], b[1]-a[1]);

		for (int l = -(int)width; l <= (int)width; l++) {
			int dx = -l*(b[1]-a[1])/norm;
			int dy = l*(b[0]-a[0])/norm;
			int p[2] = {a[0], a[1]};
			if (abs(b[1] - a[1]) > b[0] - a[0]) {
				while (p[1] != b[1]) {
					GrBytep bp = img->rows[p[1]+dy] + 3*(p[0]+dx);
					*(GrColor *)bp = color;

					if (b[1] > a[1]) {
						if (p[1]*(b[0]-a[0]) - p[0]*(b[1]-a[1]) > c1) {
							p[0]++;
						} else {
							p[1]++;
						}
					} else {
						if (p[1]*(b[0]-a[0]) - p[0]*(b[1]-a[1]) < c1) {
							p[0]++;
						} else {
							p[1]--;
						}
					}
				}
			} else {
				while (p[0] != b[0]) {
					GrBytep bp = img->rows[p[1]+dy] + 3*(p[0]+dx);
					*(GrColor *)bp = color;

					if (b[1] > a[1]) {
						if (p[1]*(b[0]-a[0]) - p[0]*(b[1]-a[1]) < c1) {
							p[1]++;
						} else {
							p[0]++;
						}
					} else {
						if (p[1]*(b[0]-a[0]) - p[0]*(b[1]-a[1]) > c1) {
							p[1]--;
						} else {
							p[0]++;
						}
					}
				}
			}

		}
	}
	return 0;
}

int grDrawCircle (GrImage *img, int p[2], int radius, GrColor color, int width) {
	int v[2];
	double dt = 1.4 / radius;
	for (double t = 0; t < 2 * M_PI; t += dt) {
		v[0] = p[0] + radius*cos(t);
		v[1] = p[1] + radius*sin(t);
		grPutDot (img, v, color, width);
	}
	return 0;
}

// the following fns are needed for grDrawPolygon
static int compare_y (const void *a, const void *b) {
	const int ia = *(int *)a;
	const int ib = *(int *)b;

	if (array_copy[2*ia+1] > array_copy[2*ib+1]) {
		return 1;
	} else if (array_copy[2*ia+1] < array_copy[2*ib+1]) {
		return -1;
	}
	return 0;
}

static int compare_line_x (const void *a, const void *b) {
	const int *ia = a;
	const int *ib = b;

	if (array_copy[2*ia[0]] > array_copy[2*ib[0]] ||
			array_copy[2*ia[1]] > array_copy[2*ib[1]]) {
		return 1;
	} else if (array_copy[2*ia[0]] < array_copy[2*ib[0]] ||
			array_copy[2*ia[1]] < array_copy[2*ib[1]]) {
		return -1;
	}
	return 0;
}

static inline int modulo (int n, int base) {
	int ans = n%base;
	if (ans < 0) ans += base;
	return ans;
}

int grDrawPolygon (GrImage *img, const int *verts, const int n, GrColorScheme *scheme) {
	int retval = -1;
	array_copy = verts;
	int *indices = malloc (n*sizeof(int));
	if (!indices) {
		SLOG_ERROR("grDrawPolygon", "can't allocate memory for indices");
		goto _bailout;
	}

	retval = -2;

	for (int i = 0; i < n; i++) {
		indices[i] = i;
	}

	// compare function depends on array_copy
	qsort (indices, n, sizeof(int), &compare_y);

	// polygon is devided into horizontal trapeziums
	// side is reprezented by two points
	// and top is counted with i, while (i+1) is bottom
	// area between sides is colored
#define BUF_DELTA 16
	int line_buf_size = BUF_DELTA;
	int line_count = 0;
	int *line = malloc (line_buf_size*sizeof(int));
	if (!line) {
		SLOG_ERROR("grDrawPolygon", "can't allocate line");
		goto _bailout;
	}

	retval = -3;

	for (int i = 0; i < n;) {
		if (verts[2*indices[i]+1] == verts[2*indices[n-1]+1]) break;

		for (int j = 0; j < line_count; j += 2) {
			if (verts[2*line[j+1]+1] <= verts[2*indices[i]+1]) {
				for (int k = j+2; k < line_count; k++) {
					line[k-2] = line[k];
				}
				line_count -= 2;
				j -= 2;
			}
		}
		{
			line_buf_size = (line_count / BUF_DELTA + 1) * BUF_DELTA;
			void *p = realloc (line, line_buf_size*sizeof(int));
			if (!p) {
				SLOG_ERROR("grDrawPolygon", "can't reallocate line(1)");
				goto _bailout;
			}
		}
		int j;
		for (j = i; verts[2*indices[i]+1] == verts[2*indices[j]+1] && j < n; j++) {
			if (verts[2*((indices[j]+1)%n)+1] > verts[2*indices[i]+1]) {
				if (line_count == line_buf_size) {
					line_buf_size += BUF_DELTA;
					void *p = realloc (line, line_buf_size*sizeof(int));
					if (!p) {
						SLOG_ERROR("grDrawPolygon", "can't reallocate line(2)");
						goto _bailout;
					}
					line = p;
				}
				line[line_count] = indices[j];
				line[line_count+1] = (indices[j]+1)%n;
				line_count += 2;
			}
			if (verts[2*modulo(indices[j]-1, n)+1] > verts[2*indices[i]+1]) {
				if (line_count == line_buf_size) {
					line_buf_size += BUF_DELTA;
					void *p = realloc (line, line_buf_size*sizeof(int));
					if (!p) {
						SLOG_ERROR("grDrawPolygon", "can't reallocate line(3)");
						goto _bailout;
					}
					line = p;
				}
				line[line_count] = indices[j];
				line[line_count+1] = modulo (indices[j]-1, n);
				line_count += 2;
			}
		}
#undef BUF_DELTA
		qsort (line, line_count/2, 2*sizeof(int), &compare_line_x);
		//		printf ("!lines %d (i = %d):\n", (p[a[i]])[1], i);
		//		for (int l = 0; l < line_count; l += 2)
		//			printf ("(%d, %d) -> (%d, %d)\n", (p[line[l]])[0], (p[line[l]])[1], (p[line[l+1]])[0], (p[line[l+1]])[1]);
		//		putchar ('\n');

		for (int l = 0; l < line_count; l += 3) {
			double c1 = (double)(verts[2*line[l+1]] - verts[2*line[l]]) /
				((verts[2*line[l+1]+1]) - verts[2*line[l]+1]);
			double d1 = (double)verts[2*line[l]] - c1*verts[2*line[l]+1];

			double c2 = (double)(verts[2*line[l+3]] - verts[2*line[l+2]]) /
				(verts[2*line[l+3]+1] - verts[2*line[l+2]+1]);
			double d2 = (double)verts[2*line[l+2]] - c2*verts[2*line[l+2]+1];

			for (int k = verts[2*indices[i]+1]; k < verts[2*indices[j]+1] && k < img->height; k++) {
				GrByte *bp = img->rows[k];
				for (int m = c1*k + d1; m <= c2*k + d2 && m < img->width; m++) {
					bp[m*3] = 50;
					bp[m*3+1] = 55;
					bp[m*3+2] = 55;
				}
			}
		}
		i = j;
	}
	// last line
	if (verts[2*line[1]+1] < img->height) {
		GrByte *bp = img->rows[verts[2*line[1]+1]];
		for (int l = 0; l < line_count; l += 3) {
			for (int k = verts[2*line[l+1]]; k <= verts[2*line[l+3]] && k < img->width; k++) {
				bp[k*3] = 50;
				bp[k*3+1] = 55;
				bp[k*3+2] = 55;
			}
		}
	}
	retval = 0;
_bailout:

	if (indices) {
		free (indices);
	}
	if (line) {
		free (line);
	}

	return retval;
}
