/*
 * Copyright ©2019 Jeffery Tian.  All rights reserved.
 * Idk how to do this copyright stuff. Use it if you want.
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>
#include <jpeglib.h>
#include <string.h>

using std::shared_ptr;
using std::string;

class Image {
 public:
  // Constructor that reads an image to a file
  explicit Image(const char *inputName);
  explicit Image(const char *fileName, int width,
            int height, unsigned char *raw);
  Image(const Image &other);
  int saveImage(const char *outputName, int quality);
  bool isValid() {return valid_;}
  bool blackAndWhite(const char *outputName, int quality);
 private:
  int getIndex(char color, int x, int y);
  void cut();
  bool valid_;
  shared_ptr<unsigned char[]> raw_image_;
  string input_name_;
  int width_;
  int height_;
  int bytes_per_pixel_;
  int color_space_;
};

#endif  // IMAGE_H_
