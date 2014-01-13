/*
 * Desc: png read/write wrapper.
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 * Copyright 2014- Li XianJing <xianjimli@hotmail.com>
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#ifndef _PNG_IMAGE_H
#define _PNG_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ImageData {
	int bit_depth;
	int color_type;
	unsigned int width;
	unsigned int height;
	unsigned int rowbytes;
	unsigned char** rows;
}ImageData;

/**
 * Load @file_name into ImageData, return it.
 *
 * If it is ok return ImageData, else return NULL.
 */
ImageData* png_image_load(char* file_name);

/**
 * Save ImageData into @file_name. 
 *
 * If it is ok return 1, else return 0. 
 */
int png_image_save(char* file_name, ImageData* image);

/**
 * Destroy ImageData
 */
void png_image_destroy(ImageData* image);

#ifdef __cplusplus
}
#endif

#endif/*_PNG_IMAGE_H*/

