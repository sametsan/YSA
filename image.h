#include "config.h"

#ifndef IMAGE_H
#define IMAGE_H


typedef struct {

    char folder[100];
	char file[50];
    char inputs_file[150];
	unsigned char *data;
	unsigned char **gray;
	char **map;
	int *hash;
	int width;
	int height;
	int datasize;
    int hashsize;
    int *maptagramX;
    int *maptagramY;

    int trim_width;
    int trim_height;
    unsigned char **trim;
}image;


typedef struct {
    unsigned short type;              // Magic identifier: 0x4d42
    unsigned int size;              // File size in bytes
    unsigned short reserved1;         // Not used
    unsigned short reserved2;         // Not used
    unsigned int offset;            // Offset to image data in bytes from beginning of file
    unsigned int dib_header_size;   // DIB Header size in bytes
    int  width_px;          		// Width of the image
    int  height_px;         		// Height of image
    unsigned short num_planes;        // Number of color planes
    unsigned short bits_per_pixel;    // Bits per pixel
    unsigned int compression;       // Compression type
    unsigned int image_size_bytes;  // Image size in bytes
    int  x_resolution_ppm;  // Pixels per meter
    int  y_resolution_ppm;  // Pixels per meter
    unsigned int num_colors;        // Number of colors
    unsigned int important_colors;  // Important colors
} BMPHeader;




void img_read(image *img);
void img_map(image *img);
void img_mapsave(image *img);
void img_free(image *img);
void img_init(image *img);
void img_hashsave(image *img);
void img_hash(image *img);
void img_maptagram(image *img);
void img_trim(image *img);

#endif

