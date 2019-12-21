#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstdlib>
#include <jpeglib.h>
#include <string>

using std::string;

class Image {
 public:
  explicit Image(const char * inputName);
  Image(const Image &other);
  ~Image();
  void saveImage(const char *outputName);
 private:
  char *raw_image_;
  string input_name_;
  int width_;
  int height_;
};

#endif  // IMAGE_H_
