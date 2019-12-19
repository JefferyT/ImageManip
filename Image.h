#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

typedef struct ImageData {
  int width;
  int height;
  unsigned char * raw;
} ImageData;

typedef enum {R, G, B} PixelComponent;

ImageData* loadJpg(const char* Name);
int saveJpg(ImageData* image, const char* name);
unsigned char getValue(PixelComponent component, int x, int y);
