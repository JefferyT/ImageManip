#include <stdio.h>
#include <jpeglib.h>

typedef struct ImageData {
	int width;
	int height;
	unsigned char * a;
	unsigned char * r;
	unsigned char * g;
	unsigned char * b;
} ImageData;

ImageData* loadJpg(const char* Name);
