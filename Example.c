#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
/* we will be using this uninitialized pointer later to store raw, uncompressd image */
unsigned char *raw_image = NULL;
/* dimensions of the image we want to write */

int width = 680;
int height = 480;
int bytes_per_pixel = 3; /* or 1 for GRACYSCALE images */
int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

/**
* read_jpeg_file Reads from a jpeg file on disk specified by filename and saves into the
* raw_image buffer in an uncompressed format.
*
* \returns positive integer if successful, -1 otherwise
* \param *filename char string specifying the file name to read from
*
*/

int read_jpeg_file(char *filename)
{

	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1];
	FILE *infile = fopen(filename, "rb");
	unsigned long location = 0;
	int i = 0;
	if (!infile)
	{
		printf("Error opening jpeg file %s\n!", filename);
		return -1;
	}

	/* here we set up the standard libjpeg error handler */
	cinfo.err = jpeg_std_error(&jerr);

	/* setup decompression process and source, then read JPEG header */
	jpeg_create_decompress(&cinfo);

	/* this makes the library read from infile */
	jpeg_stdio_src(&cinfo, infile);

	/* reading the image header which contains image information */
	jpeg_read_header(&cinfo, TRUE);

	/* Uncomment the following to output image information, if needed. */
	
	printf( "JPEG File Information: \n" );
	printf( "Image width and height: %d pixels and %d pixels.\n", cinfo.image_width, cinfo.image_height );
	printf( "Color components per pixel: %d.\n", cinfo.num_components );
	printf( "Color space: %d.\n", cinfo.jpeg_color_space );
	

	/* Start decompression jpeg here */
	jpeg_start_decompress(&cinfo);

	/* allocate memory to hold the uncompressed image */
	raw_image = (unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.num_components);

	/* now actually read the jpeg into the raw buffer */
	row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);

	/* read one scan line at a time */
	while (cinfo.output_scanline < cinfo.image_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (i = 0; i < cinfo.image_width*cinfo.num_components; i++)
			raw_image[location++] = row_pointer[0][i];
	}

	/* wrap up decompression, destroy objects, free pointers and close open files */
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	fclose(infile);

	/* yup, we succeeded! */
	return 1;
}

/**
* write_jpeg_file Writes the raw image data stored in the raw_image buffer
* to a jpeg image with default compression and smoothing options in the file
* specified by *filename.
* \returns positive integer if successful, -1 otherwise
* \param *filename char string specifying the file name to save to
*
*/

int write_jpeg_file(char *filename)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];
	FILE *outfile = fopen(filename, "wb");
	if (!outfile)
	{
		printf("Error opening output jpeg file %s\n!", filename);
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
	
	/* Setting the parameters of the output file here */
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = bytes_per_pixel;
	cinfo.in_color_space = color_space;

	/* default compression parameters, we shouldn't be worried about these */
	jpeg_set_defaults(&cinfo);
	cinfo.num_components = 3;

	//cinfo.data_precision = 4;
	cinfo.dct_method = JDCT_FLOAT;
	jpeg_set_quality(&cinfo, 100, TRUE);

	/* Now do the compression .. */
	jpeg_start_compress(&cinfo, TRUE);
	/* like reading a file, this time write one row at a time */

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &raw_image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);

	/* success code is 1! */

	return 1;
}

int main()
{
	char *infilename = "test.jpg", *outfilename = "test_out.jpg";
	/* Try opening a jpeg*/

	if (read_jpeg_file(infilename) > 0)
	{
		/* then copy it to another file */
		if (write_jpeg_file(outfilename) < 0) return -1;
	}

	else return -1;

	return 0;

}
