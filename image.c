#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "bmp24/bmp.h"
const char *err_msgs[ERRORS_AMOUNT] = {
	"Success",
	"Can't open image file",
	"Can't read image file",
	"Can't write image file",
	"File is not an image",
	"Wrong image header"
};

void lib_init(){
	operations[0] = bmp_ops;
}

int read_image(const char *imagepath, image_t *image){
	int result;
	uint16_t type;

	FILE *im = fopen(imagepath, "rb");
	if ( im == NULL ){
		return EOPENFILE;
	}

	/* Get type signature */
	result = get_type(im, &type);
	if( result != SUCCESS ){
		return result;
	}

	/* Get operations for detected type */
	result = get_spec_ops(type, &image->ops);
	if( result != SUCCESS ){
		return result;
	}

	/* Read image header */
	result = image->ops->read_spec_head(im, image);
	if( result != SUCCESS ){
		return result;
	}

	/* Read image pixels */
	result = image->ops->read_spec_body(im, image);
	if( result != SUCCESS ){
		return result;
	}

	fclose(im);
	return SUCCESS;
}

int get_type(FILE *image, uint16_t *type){
	int type_bytes = fread(type, 1, sizeof(uint16_t), image);
	if( type_bytes < SIGN_SIZE ){
		return EREAD;
	}
	/* Reset position to simplify reading header in callbacks */
	rewind(image);
	return SUCCESS;
}

int get_spec_ops(uint16_t type, spec_ops_t **current_ops){
	/* Using an array of callbacks */
	int i;
	*current_ops = NULL;
	for (i = 0; i < SUPPORTED_FORMATS_NUM; i++){
		if( operations[i].type == type ){
			*current_ops = &operations[i];
		}
	}
	if( *current_ops == NULL ){
		return ENOIMAGE;
	}
	return SUCCESS;
}

int write_image(const char *imagepath, image_t *image){
	int result;
	FILE *im = fopen(imagepath, "wb");
	if ( im == NULL ){
		return EOPENFILE;
	}

	/* Write image header */
	result = image->ops->write_spec_head(im, image);
	if( result != SUCCESS ){
		return result;
	}

	/* Write image pixels */
	result = image->ops->write_spec_body(im, image);
	if( result != SUCCESS ){
		return result;
	}
	fclose(im);
	return SUCCESS;
}

static int min(int a, int b){
	return (a < b) ? a : b;
}

static int max(int a, int b){
	return (a > b) ? a : b;
}

int rotate_image(image_t *image, int32_t angle){
	int x, y, old_width = image->width, old_height = image->height;
	pixel_t *new_pixels;

	float radians = (2 * M_PI * angle) / 360; 
	
	float cosine = (float) cos(radians); 
	float sine = (float) sin(radians); 
	
	int x1 = ceil(-old_height * sine); 
	int y1 = ceil(old_height * cosine); 
	int x2 = ceil(old_width * cosine - old_height * sine); 
	int y2 = ceil(old_height * cosine + old_width * sine); 
	int x3 = ceil(old_width * cosine); 
	int y3 = ceil(old_width * sine); 
	
	int min_x = min(0,  min(x1, min(x2, x3))); 
	int min_y = min(0,  min(y1, min(y2, y3))); 
	int max_x = max(x1, max(x2, x3)); 
	int max_y = max(y1, max(y2, y3)); 
	
	int dest_width = max_x - min_x;
	int dest_height = max_y - min_y;

	image->width = dest_width;
	image->height = dest_height;
	
	new_pixels = malloc(image->width*image->height*sizeof(pixel_t));

	for(y = 0; y < dest_height; y++){
		for(x = 0; x < dest_width; x++ ){
			int sourcex = ceil((x+min_x)*cosine + (y+min_y)*sine);
			int sourcey = ceil((y+min_y)*cosine - (x+min_x)*sine);
			if( sourcex >= 0 && sourcex < old_width
				&& sourcey >= 0 && sourcey < old_height ){
				new_pixels[y*dest_width+x] = image->pixels[sourcey*old_width+sourcex];
			}else{
				pixel_t nul = {0xff, 0xff, 0xff};
				new_pixels[y*dest_width+x] = nul;
			}
		}
	}

	free(image->pixels);

	image->pixels = new_pixels;

	return SUCCESS;
}

const char *get_error_msg(img_errors_t errno){
	if( errno >= SUCCESS && errno <= EWRONGHEAD ){
		return err_msgs[errno];
	}
	return err_msgs[SUCCESS];
}