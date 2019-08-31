#include "Image.h" 


ImageData* loadJpg(const char* Name) {
  ImageData * ret = new ImageData;
  unsigned char a, r, g, b;
  int width, height;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  ret->a = new unsigned char[height*width];
  ret->r = new unsigned char[height*width];
  ret->g = new unsigned char[height*width];
  ret->b = new unsigned char[height*width];
  FILE * infile;        /* source file */
  JSAMPARRAY pJpegBuffer;       /* Output row buffer */
  int row_stride;       /* physical row width in output buffer */
  if ((infile = fopen(Name, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", Name);
    return 0;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  width = cinfo.output_width;
  height = cinfo.output_height;
  
  ret->width = width;
  ret->height = height;
  
  int index = 0;
  if (!(ret->a && ret->r && ret->g && ret->b)) {
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
	  ret->a[index] = a;
	  ret->r[index] = r;
	  ret->g[index] = g;
	  ret->b[index] = b;
    }
  }
  fclose(infile);
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  return ret;
}
