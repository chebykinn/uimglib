#ifndef IMAGE_H
#define IMAGE_H
#define SIGN_SIZE 2
#define SUPPORTED_FORMATS_NUM 1
#define ERRORS_AMOUNT 6

#define LIB_VERSION "1.0"
typedef enum img_errors_t{
	SUCCESS,
	EOPENFILE,
	EREAD,
	EWRITE,
	ENOIMAGE,
	EWRONGHEAD
} img_errors_t;

extern const char *err_msgs[ERRORS_AMOUNT]; 

typedef struct image_t image_t;

typedef int (*op_callback_t)(FILE *imagefile, image_t *image);

typedef struct spec_ops_t{
	uint16_t type;
	op_callback_t read_spec_head;
	op_callback_t read_spec_body;

	op_callback_t write_spec_head;
	op_callback_t write_spec_body;
} spec_ops_t;

spec_ops_t operations[SUPPORTED_FORMATS_NUM];

typedef struct pixel_t{
	uint8_t b;
	uint8_t g;
	uint8_t r;
} pixel_t;

struct image_t{
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t offset;
	spec_ops_t *ops;
	pixel_t *pixels;
};


void lib_init();
int read_image(const char *imagepath, image_t *image);
int get_type(FILE *image, uint16_t *type);
int get_spec_ops(uint16_t type, spec_ops_t **current_ops);

int rotate_image(image_t *image, int32_t angle);

int write_image(const char *imagepath, image_t *image);

const char *get_error_msg(img_errors_t errno);
#endif