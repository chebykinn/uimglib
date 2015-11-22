#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../image.h"
#include "bmp.h"
spec_ops_t bmp_ops = { 
	BMP_SIGN,
	read_bmp_head,
	read_bmp_body,
	write_bmp_head,
	write_bmp_body
};

int read_bmp_head(FILE *imagefile, image_t *image){
	size_t length;
	bmp_header_t header;
	length = fread(&header, 1, sizeof(bmp_header_t), imagefile);

	if( length == BMP_HEADER_SIZE && header.bfType == BMP_SIGN && header.biBitCount == BMP_24){
		/* Header was loaded successfully */
		image->width = header.biWidth;
		image->height = header.biHeight;
		image->depth = BMP_24;
		image->offset = header.bOffBits;
		return SUCCESS;
	}

	return EWRONGHEAD;
}

int read_bmp_body(FILE *imagefile, image_t *image){
	uint32_t y, left, width = image->width, height = image->height;

	/* Set position to pixels data */
	fseek(imagefile, image->offset, SEEK_SET);
	left = width % 4;
	image->pixels = malloc(width*height*sizeof(pixel_t));
	
	for(y = 0; y < height; y++){
		size_t res;
		res = fread(&image->pixels[y*width], width*sizeof(pixel_t), 1, imagefile);
		if( res < 1 ){
			return EREAD;
		}
		/* Skip paddings */
		fseek(imagefile, left, SEEK_CUR);
	}
	return SUCCESS;
}

int write_bmp_head(FILE *imagefile, image_t *image){
	uint32_t line, size, length;
	bmp_header_t header;

	line = floor( (image->depth * image->width + 31) / 32 ) * 4;
	size = line * image->height;
	header.bfType = BMP_SIGN;
	header.bfileSize = size + image->offset;
	header.bfReserved = 0;
	header.bOffBits = image->offset;
	header.biSize = BMP_HEADER_SIZE*2;
	header.biWidth = image->width;
	header.biHeight = image->height;
	header.biPlanes = 1;
	header.biBitCount = image->depth;
	header.biCompression = 0;
	header.biSizeImage = size;
	header.biXPelsPerMeter = PPM_96_DPI;
	header.biYPelsPerMeter = PPM_96_DPI;
	header.biClrUsed = 0;
	header.biClrImportant = 0;

	length = fwrite(&header, 1, sizeof(bmp_header_t), imagefile);

	if( length < BMP_HEADER_SIZE ){
		return EWRITE;
	}
	return SUCCESS;
}

int write_bmp_body(FILE *imagefile, image_t *image){
	uint32_t y, width = image->width, height = image->height, left = width % 4;

	fseek(imagefile, image->offset, SEEK_SET);
	for(y = 0; y < height; y++){
		size_t res = fwrite(&image->pixels[y*width], width*sizeof(pixel_t)+left, 1, imagefile);
		if( res < 1 ){
			return EWRITE;
		}
	}
	return SUCCESS;
}