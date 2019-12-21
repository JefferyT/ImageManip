#include "Image.h" 

char getIndex(PixelComponent comp, ImageData* rawImage, int x, int y) {
  int index = (y * rawImage->width + x) * 3;
  if (comp == R) {
	return index;
  } else if (comp == G) {
	return index + 1;
  } else if (comp == B) {
	return index + 2;
  } else {
	return -1;
  }
}

ImageData* loadJpg(const char* name) {
  ImageData *ret = new ImageData();
  unsigned char a, r, g, b;
  int width, height;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  
  
  FILE * infile;        /* source file */
  JSAMPARRAY pJpegBuffer;       /* Output row buffer */
  int row_stride;       /* physical row width in output buffer */
  if ((infile = fopen(name, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", name);
    return 0;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  width = cinfo.output_width;
  height = cinfo.output_height;
  
  ret->width = width;
  ret->height = height;

  ret->raw = new unsigned char[sizeof(char) * width * height * 3];

  int index = 0;
  if (!(ret->raw)) {
    printf("NO MEM FOR JPEG CONVERT!\n");
    return 0;
  }
  row_stride = width * cinfo.output_components;
  pJpegBuffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
 
  while (cinfo.output_scanline < cinfo.output_height) {
	(void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
    for (int x = 0; x < width; x++) {
      a = 0; // alpha value is not supported on jpg
      r = pJpegBuffer[0][cinfo.output_components * x];
      if (cinfo.output_components > 2) {
        g = pJpegBuffer[0][cinfo.output_components * x + 1];
        b = pJpegBuffer[0][cinfo.output_components * x + 2];
      } else {
        g = r;
        b = r;
      }
	  ret->raw[index] = r;
      ret->raw[index+1] = g;
      ret->raw[index+2] = b;
	  index++;
    }
  }
  fclose(infile);
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  return ret;
}

int saveJpg(ImageData* raw, const char* filename) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  /* pointer to one row of image data */
  JSAMPROW row_pointer[1];
  FILE *outfile = fopen(filename, "wb");
  if (!outfile) {
  printf("Error opening output jpeg file %s\n!", filename);
    return -1;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);

  // Setting the parameters of the output file here
  cinfo.image_width = raw->width;
  cinfo.image_height = raw->height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  cinfo.num_components = 3;

  cinfo.dct_method = JDCT_FLOAT;
  jpeg_set_quality(&cinfo, 15, TRUE);

  jpeg_start_compress(&cinfo, TRUE);
  
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &raw->raw[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  fclose(outfile);

  return 1;
}
