#ifndef IMAGE_H
#define IMAGE_H

typedef enum img_errors{
	EREAD,
	EWRITE,
	EWRONGHEAD
} img_errors;

typedef struct image_t image_t;

typedef int (*op_callback)(FILE *imagefile, image_t *image);

typedef struct spec_ops{
	uint16_t type;
	op_callback read_spec_head;
	op_callback read_spec_body;

	op_callback write_spec_head;
	op_callback write_spec_body;
} spec_ops;

typedef struct pixel_t{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel_t;

struct image_t{
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t offset;
	spec_ops *ops;
	pixel_t *pixels;
};


int read_image(const char *imagepath, image_t *image);
int get_type(FILE *image, uint16_t *type);
int get_spec_opts(uint16_t *type, spec_ops *current_ops); /* an array of callbacks */

int rotate(image_t *image_src, image_t *image_dest);

int write_image(const char *imagepath, image_t *image);
#endif