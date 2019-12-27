/*
 * Copyright ©2019 Jeffery Tian.  All rights reserved.
 * Idk how to do this copyright stuff. Use it if you want.
 */

#include "Image.h"

#define DEFAULT_NUM_COMPONENTS 3

using std::cerr;
using std::endl;

Image::Image(const char *inputName) {
  valid_ = false;
  if (inputName == nullptr) {
    return;
  }
  string name(inputName);
  input_name_ = name;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  JSAMPROW row_pointer[1];
  FILE *infile = fopen(input_name_.c_str(), "rb");
  uint64_t location = 0;
  unsigned int i = 0;
  if (!infile) {
    cerr << "Error opening jpeg file " << input_name_ << "!" << endl;
  }

  // Setting up the standard libjpeg error handler
  cinfo.err = jpeg_std_error(&jerr);

  // setup decompression process and source, and then read JPEG header
  jpeg_create_decompress(&cinfo);

  // The library reads from infile
  jpeg_stdio_src(&cinfo, infile);

  // Read the image header which contains image information
  jpeg_read_header(&cinfo, TRUE);

  // Decompress jpeg
  jpeg_start_decompress(&cinfo);

  this->width_ = cinfo.output_width;
  this->height_ = cinfo.output_height;
  this->bytes_per_pixel_ = cinfo.num_components;
  this->color_space_ = cinfo.out_color_space;

  this->raw_image_ = new unsigned char[cinfo.output_width *
              cinfo.output_height * cinfo.num_components];

  row_pointer[0] = new unsigned char[cinfo.output_width *
              cinfo.num_components];

  while (cinfo.output_scanline < cinfo.image_height) {
    jpeg_read_scanlines(&cinfo, row_pointer, 1);
    for (i = 0; i < cinfo.image_width * cinfo.num_components; i++) {
      this->raw_image_[location++] = row_pointer[0][i];
    }
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  delete[] row_pointer[0];
  fclose(infile);
  valid_ = true;
}

Image::Image(const char *fileName, int width,
          int height, unsigned char *raw) :
          width_(width), height_(height) {
  valid_ = false;
  if (width * height * DEFAULT_NUM_COMPONENTS != sizeof(raw)/sizeof(raw[0])) {
    return;
  }
  strncpy(reinterpret_cast<char *>(this->raw_image_), reinterpret_cast<char *>(raw),
            width_ * height_ * DEFAULT_NUM_COMPONENTS);
}

Image::Image(const Image &other) : input_name_(other.input_name_),
            width_(other.width_), height_(other.height_),
            bytes_per_pixel_(other.bytes_per_pixel_),
            color_space_(other.color_space_) {
  this->raw_image_ = new unsigned char[other.width_ * other.height_];
  if (this->raw_image_ == nullptr) {
    valid_ = false;
    return;
  }
  strncpy((char *) this->raw_image_, (char *) other.raw_image_,
            other.width_ * other.height_ * this->bytes_per_pixel_);
  this->valid_ = true;
}

Image::~Image() {
  if (this->isValid()) {
    delete[] this->raw_image_;
  }
}

int Image::saveImage(const char *outputName, int quality) {
  struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  FILE *outfile = fopen(outputName, "wb");
  if (!outfile) {
    cerr << "Error opening output jpeg file " << outputName << endl;
    return -1;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = width_;
  cinfo.image_height = height_;
  cinfo.input_components = this->bytes_per_pixel_;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  cinfo.num_components = 3;

  cinfo.dct_method = JDCT_FLOAT;
  jpeg_set_quality(&cinfo, quality, true);

  jpeg_start_compress(&cinfo, TRUE);

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &(this->raw_image_[cinfo.next_scanline
            * cinfo.image_width * cinfo.input_components]);
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  fclose(outfile);

  return 1;
}

int Image::getIndex(char color, int x, int y) {
  int offset = -1;
  if (color == 'r' || color == 'R') {
    offset = 0;
  } else if (color == 'g' || color == 'G') {
    offset = 1;
  } else if (color == 'b' || color == 'B') {
    offset = 2;
  } else {
    return offset;
  }
  return (this->width_ * y + x) * DEFAULT_NUM_COMPONENTS + offset;
}

bool Image::blackAndWhite(const char *outputName, int quality) {
  Image bW(*this);
  for (int i = 0; i < bW.height_; i++) {
    for (int j = 0; j < bW.width_; j++) {
      int rgbI[3];
      float bAndW = '0';
      double weights[3] = {0.299, 0.587, 0.114};
      rgbI[0] = bW.getIndex('r', j, i);
      rgbI[1] = bW.getIndex('g', j, i);
      rgbI[2] = bW.getIndex('b', j, i);
      for (int k = 0; k < 3; k++) {
        bAndW += weights[k] + bW.raw_image_[rgbI[k]];
      }
      bAndW += 0.5;
      int rounded = (int) bAndW;
      unsigned char val = rounded;
      for (int k = 0; k < 3; k++) {
        bW.raw_image_[rgbI[k]] = val;
      }
    }
  }
  bW.saveImage(outputName, quality);
  bW.cut();
  return true;
}

void Image::cut() {
  for (int i = 0; i < width_ * height_ * DEFAULT_NUM_COMPONENTS; i++) {
    if (raw_image_[i] > 255) {
      raw_image_[i] = 255;
    }
  }
}