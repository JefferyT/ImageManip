CC = g++
all: PhotoMod.cpp PhotoMod.h Image.cpp
	$(CC) -g -c PhotoMod.cpp Image.cpp
	$(CC) -g -Wall -o PhotoMod PhotoMod.o Image.o -ljpeg
