/*
 * Copyright ©2019 Jeffery Tian.  All rights reserved.
 * Idk how to do this copyright stuff. Use it if you want.
 */

#include <stdio.h>
#include <cstdlib>
#include <string>
#include "Image.h"

using std::string;

int main(int argc, char** argv) {
  if (argc == 2) {
    string outFile = "out.jpg";
    Image img(argv[1]);
    if (!img.isValid()) {
      return EXIT_FAILURE;
    }
    Image img2(img);
    if (!img2.isValid()) {
      return EXIT_FAILURE;
    }
    // img.saveImage(outFile.c_str(), 100);
    // img.blackAndWhite("bandw.jpg", 100);
  }
}
