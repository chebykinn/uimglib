#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
		fclose(im);
		return result;
	}

	/* Get operations for detected type */
	result = get_spec_ops(type, &image->ops);
	if( result != SUCCESS ){
		fclose(im);
		return result;
	}

	/* Read image header */
	result = image->ops->read_spec_head(im, image);
	if( result != SUCCESS ){
		fclose(im);
		return result;
	}

	/* Read image pixels */
	result = image->ops->read_spec_body(im, image);
	if( result != SUCCESS ){
		fclose(im);
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
		fclose(im);
		return result;
	}

	/* Write image pixels */
	result = image->ops->write_spec_body(im, image);
	if( result != SUCCESS ){
		fclose(im);
		return result;
	}
	fclose(im);
	return SUCCESS;
}

int rotate_image(image_t *image, int32_t angle){
	uint32_t x, y, old_width = image->width, old_height = image->height;
	pixel_t *new_pixels;
	new_pixels = malloc(old_width*old_height*sizeof(pixel_t));
	
	image->width = old_height;

	image->height = old_width;

	for (y = 0; y < old_height; y++){
		for (x = 0; x < old_width; x++){
			uint32_t new_x, new_y;
			new_x = (old_height - 1) - y;
			new_y = x;
			new_pixels[new_y*old_height + new_x] = image->pixels[y*old_width + x];
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