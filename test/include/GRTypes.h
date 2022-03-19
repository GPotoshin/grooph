/*
 * =====================================================================================
 *
 *        Filename:  GRTypes.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/14/2022 13:50:23
 *       Revision:  none
 *       Compiler:  
 *
 *         Author:  George Potoshin (GP), george.potoshin@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GRTYPES_H
#define GRTYPES_H

typedef unsigned char GRByte;
typedef unsigned char * GRBytep;

typedef struct {
	int filetype;

	int colortype;
	int interlace;
	int compression;
	int filter;

	char *libver;
	
	int height;
	int width;

	GRBytep *rows;

	void **data;
	int partition;
} GRImg;

typedef struct {
	int x;
	int y;
} GRInt2;

typedef struct {
	GRByte red;
	GRByte green;
	GRByte blue;
} GRColor;

#endif /* GRTYPES_H */
