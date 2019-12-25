CC=gcc -Wall -g -std=c11
PP=g++ -Wall -g -std=c++11
COMP=-c
LINK=-o

all: PhotoMod Example

Example: Example.c
	$(CC) $(LINK) Example Example.c -ljpeg

PhotoMod: PhotoMod.o Image.o
	$(PP) $(LINK) PhotoMod PhotoMod.o Image.o -ljpeg

PhotoMod.o: PhotoMod.cc Image.h
	$(PP) $(COMP) PhotoMod.cc

Image.o: Image.cc Image.h
	$(PP) $(COMP) Image.cc

clean:
	rm -f *.o *~ PhotoMod