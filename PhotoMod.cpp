/*
 * Made by Jeffery Tian
 *
 */
#include <stdio.h>
#include "Image.h"

int main(int argc, char** argv) {
	printf("%s\n", argv[1]);
	FILE *fp;
	if ((fp = fopen(argv[1], "r")) != NULL) {
		int c;
		while ((c = getc(fp)) != EOF) {
			printf("%c",c);
		}
	}
	fclose(fp);
}
int func() {
	return 0;
}
