/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 * Copyright 2014- Li XianJing <xianjimli@hotmail.com>
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#define PNG_DEBUG 3

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <png.h>
#include "png_image.h"

#define return_val_if_fail(p, val) if(!(p)) \
	{ printf("%s:%d "#p" failed.\n", __func__, __LINE__); return (val);}

ImageData* png_image_load(char* file_name)
{
	unsigned int y = 0;
	png_byte bit_depth = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	int number_of_passes = 0;
	png_byte color_type = 0;
	ImageData* image = NULL;

	png_infop info_ptr = NULL;
	png_structp png_ptr = NULL;
	png_bytep * row_pointers = NULL;

	char header[8];	   // 8 is the maximum size that can be checked
	int rowbytes = 0;

	FILE *fp = fopen(file_name, "rb");
	return_val_if_fail(fp, NULL);

	fread(header, 1, 8, fp);
	return_val_if_fail(png_sig_cmp(header, 0, 8) == 0, NULL);

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	return_val_if_fail(png_ptr, NULL);

	info_ptr = png_create_info_struct(png_ptr);
	return_val_if_fail(info_ptr, NULL);

	return_val_if_fail(setjmp(png_jmpbuf(png_ptr)) == 0, NULL);

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	return_val_if_fail(setjmp(png_jmpbuf(png_ptr)) == 0, NULL);
	
	rowbytes = png_get_rowbytes(png_ptr,info_ptr);
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

	for (y=0; y<height; y++) 
	{
		row_pointers[y] = (png_byte*) malloc(rowbytes);
	}

	png_read_image(png_ptr, row_pointers);

	fclose(fp);

	image = malloc(sizeof(ImageData));
	image->width = width;
	image->height = height;
	image->rows = row_pointers;
	image->rowbytes = rowbytes;
	image->color_type = color_type;
	image->bit_depth = bit_depth;

	return image;
}

int png_image_save(char* file_name, ImageData* image)
{
	unsigned int y;
	unsigned int width = image->width;
	unsigned int height = image->height;
	png_byte color_type = image->color_type;
	png_byte bit_depth = image->bit_depth;
	png_bytep* row_pointers = image->rows;

	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;

	FILE *fp = fopen(file_name, "wb");
	return_val_if_fail(fp, 0);

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	return_val_if_fail(png_ptr, 0);

	info_ptr = png_create_info_struct(png_ptr);
	return_val_if_fail(info_ptr, 0);

	return_val_if_fail(setjmp(png_jmpbuf(png_ptr)) == 0, 0);
	png_init_io(png_ptr, fp);

	return_val_if_fail(setjmp(png_jmpbuf(png_ptr)) == 0, 0);

	png_set_IHDR(png_ptr, info_ptr, width, height,
		     bit_depth, color_type, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	return_val_if_fail(setjmp(png_jmpbuf(png_ptr)) == 0, 0);
	png_write_image(png_ptr, row_pointers);

	return_val_if_fail(setjmp(png_jmpbuf(png_ptr)) == 0, 0);
	png_write_end(png_ptr, NULL);

	fclose(fp);

	return 1;
}

void png_image_destroy(ImageData* image)
{
	unsigned int y = 0;

	if(image) {
		unsigned int height = image->height;
		unsigned char** row_pointers = image->rows;

		if(row_pointers) {
			for (y=0; y<height; y++)
			{
				free(row_pointers[y]);
			}
			free(row_pointers);
		}

		image->rows = NULL;
		free(image);
	}

	return;
}


#ifdef PNG_IMAGE_TEST
void process_file(ImageData* image)
{
	int x, y;
	unsigned int height = image->height;
	unsigned int width = image->width;
	png_bytep* row_pointers = image->rows;
	
	for (y=0; y<height; y++) {
			png_byte* row = row_pointers[y];
			for (x=0; x<width; x++) {
				/*TODO*/
			}
	}
}


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <file_in> <file_out>\n", argv[0]);

		return 1;
	}

	ImageData* image = png_image_load(argv[1]);
	
	process_file(image);

	png_image_save(argv[2], image);
	
	png_image_destroy(image);

	return 0;
}
#endif

