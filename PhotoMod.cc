/*
 * Made by Jeffery Tian
 *
 */
#include <stdio.h>
#include <cstdlib>
#include <string>
#include "Image.h"

using std::string;

int main(int argc, char** argv) {
/*	
	printf("%s\n", argv[1]);
	FILE *fp;
	if ((fp = fopen(argv[1], "r")) != NULL) {
		int c;
		while ((c = getc(fp)) != EOF) {
			printf("%c",c);
		}
	}
	fclose(fp);
*/
  string outFile = "out.jpg";	
  ImageData * image = loadJpg(argv[1]);
  saveJpg(image, outFile.c_str());
  delete[](image->raw);
  free(image);
}
