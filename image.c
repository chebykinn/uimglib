#include <stdio.h>
#include <stdint.h>
#include "image.h"

int read_image(const char *imagepath, image_t *image){
	int result;
	size_t length;
	uint16_t type;
	spec_ops_t current_ops;

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
	result = get_spec_ops(type, &current_ops);
	if( result != SUCCESS ){
		return result;
	}

	/* Read image header */
	result = current_ops.read_spec_head(im, image);
	if( result != SUCCESS ){
		return result;
	}

	/* Read image pixels */
	result = current_ops.read_spec_body(im, image);
	if( result != SUCCESS ){
		return result;
	}

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
int get_spec_ops(uint16_t type, spec_ops_t *current_ops){
	/* an array of callbacks */
	spec_ops_t *operations;
	int i, size;
	for (i = 0; i < size; i++){
		if( operations[i].type == type ){
			current_ops = &operations[i];
		}
	}
	return SUCCESS;
}

int write_image(const char *imagepath, image_t *image){
	return SUCCESS;
}

int rotate(image_t *image_src, image_t *image_dest){
	return SUCCESS;
}