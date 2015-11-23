#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include "image.h"

int main(int argc, char *argv[]){
	char *opts = "o:r:vV", *inputname, *outname;
	int result, opt, verbose = 0, version = 0, width = 0, height = 0;
	int32_t angle;
	image_t image;

	if( argc < 2 ){
		fprintf(stderr, "Usage: %s <src_image> [-o <dest_image>] [-r <angle>] [-vV]\n", argv[0]);
		fprintf(stderr, "  -o \t\tSpecify file to write rotated image.\n");
		fprintf(stderr, "  -r <angle> \tRotate by given angle.\n");
		fprintf(stderr, "  -v \t\tEnable output.\n");
		fprintf(stderr, "  -V \t\tDisplay version infomation.\n");
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
				angle = atoi(optarg);
			break;

			case 'v':
				verbose = 1;
			break;

			case 'V':
				version = 1;
			break;
		}
	}

	if( version ){
		printf("uimglib %s\n", LIB_VERSION);
		return 0;
	}

	lib_init();
	
	result = read_image(inputname, &image);
	width = image.width;
	height = image.height;

	if( result != SUCCESS ){
		fprintf(stderr, "%s: %s\n", inputname, get_error_msg(result));
		return 1;
	}


	result = rotate_image(&image, angle);


	result = write_image(outname, &image);
	if( result != SUCCESS ){
		fprintf(stderr, "%s: %s\n", outname, get_error_msg(result));
		return 1;
	}

	if( verbose ){
		printf("Opened %s, size: %dx%d\n", inputname, width, height);
		printf("Rotated %s by %d degrees\n", inputname, angle);
		printf("Wrote %s, size: %dx%d\n", outname, image.width, image.height);
	}

	free(image.pixels);
	return SUCCESS;
}