#ifndef BMP_H
#define BMP_H

#define BMP_HEADER_SIZE 54
#define BMP_SIGN 0x4D42
#define BMP_24 24
#define PPM_96_DPI 3780

#pragma pack(push, 2)
typedef struct {
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffBits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} bmp_header_t;
#pragma pack(pop)

int read_bmp_head(FILE *imagefile, image_t *image);
int read_bmp_body(FILE *imagefile, image_t *image);

int write_bmp_head(FILE *imagefile, image_t *image);
int write_bmp_body(FILE *imagefile, image_t *image);

extern spec_ops_t bmp_ops;
#endif