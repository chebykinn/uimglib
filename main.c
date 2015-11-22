#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include "image.h"

int main(int argc, char *argv[]){
	char *opts = "o:rv", *inputname, *outname;
	int result, opt, to_right = 0, verbose = 0, width = 0, height = 0;
	image_t image;

	if( argc < 2 ){
		fprintf(stderr, "Usage: %s <src_image> [-o <dest_image>] [-r] [-v]\n", argv[0]);
		fprintf(stderr, "  -o \tSpecify file to write rotated image.\n");
		fprintf(stderr, "  -r \tRotate by 90 degrees to the right (default is to the left).\n");
		fprintf(stderr, "  -v \tEnable output.\n");
		return 1;
	}
	inputname = argv[1];
	outname = inputname;

	while((opt = getopt(argc, argv, opts)) != -1) {
		switch(opt) {
			case 'o': 
				outname = optarg;
			break;

			case 'r':
				to_right = 1;
			break;

			case 'v':
				verbose = 1;
			break;
		}
	}
	lib_init();
	
	result = read_image(inputname, &image);
	width = image.width;
	height = image.height;

	if( result != SUCCESS ){
		fprintf(stderr, "%s: %s\n", inputname, get_error_msg(result));
		return 1;
	}


	result = rotate_image(&image, to_right);


	result = write_image(outname, &image);
	if( result != SUCCESS ){
		fprintf(stderr, "%s: %s\n", outname, get_error_msg(result));
		return 1;
	}

	if( verbose ){
		printf("Opened %s, size: %dx%d\n", inputname, width, height);
		if( !to_right ){
			printf("Rotated %s by 90 degrees to left\n", inputname);
		}else{
			printf("Rotated %s by 90 degrees to right\n", inputname);
		}
		printf("Wrote %s, size: %dx%d\n", outname, image.width, image.height);
	}

	free(image.pixels);
	return SUCCESS;
}