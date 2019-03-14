#include "texpackr/png_util.h"
#include "texpackr/log.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * define debugging information to spit out from libpng
 * higher number, more info will be given out
 * valid value is 0 to 3
 */
#define PNG_DEBUG 3

void texpackr_free_png_image_data(png_bytepp data, int height)
{
  for (int y=0; y<height; ++y)
  {
    free(data[y]);
    data[y] = NULL;
  }
  free(data);
}

png_bytepp texpackr_allocate_png_rgba_image_space(int width, int height)
{
	// for 4 channels
	int rowbytes = width * 4;
	
  png_bytepp row_ptr = (png_bytepp)malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; ++y)
  {
    row_ptr[y] = (png_bytep)calloc(1, rowbytes);
  }

	return row_ptr;
}

png_bytepp texpackr_read_png_file(const char* file_name, int* rst_rowbytes, int* rst_width, int* rst_height)
{
  // open file
  // note: libpng tells us to make sure we open in binary mode
  FILE *fp = fopen(file_name, "rb");
  if (fp == NULL)
		return NULL;

  // to hold first 8 bytes reading from png file to check if it's png file
  unsigned char header[8];

  // read the first 8 bytes (more bytes will make it more accurate)
  // but in case we want to keep our file pointer around after this
  // libpng suggests us to only read 8 bytes
  const int cmp_number = 8;
  if (fread(header, 1, cmp_number, fp) != cmp_number)
  {
    TEXPACKR_ELOG("read %s file error", file_name)

    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  // check whether magic number matches, and thus it's png file
  if (png_sig_cmp(header, 0, cmp_number) != 0)
  {
    // it's not PNG file
    TEXPACKR_ELOG("%s file is not recognized as png file\n", file_name)

    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  // create png structure
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
  {
    // cannot create png structure
    TEXPACKR_ELOG("cannot create png structure\n")

    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  // create png-info structure
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
  {
    TEXPACKR_ELOG("cannot create png info structure\n")

    // clear png resource
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  // we need to set jump callback to handle error when we enter into a new routing before the call to png_*()
  // defined as convenient for future if you every call this in different routine
  // note: if use, need to call in routine that return any pointer type
#define PNG_READ_SETJMP(png_ptr, info_ptr, fp) \
  /* set jmp */ \
  if (setjmp(png_jmpbuf(png_ptr)))  \
  { \
    TEXPACKR_ELOG("error png's set jmp for read\n") \
                                              \
    /* clear png resource */                  \
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);   \
                                                                      \
    /* close file */ \
    fclose(fp);     \
    fp = NULL;      \
    return NULL;    \
  }

  // call this once as all relevant operations all happen in just one routine
  PNG_READ_SETJMP(png_ptr, info_ptr, fp)

  // set up input code
  png_init_io(png_ptr, fp);
  // let libpng knows that we have read first initial bytes to check whether it's png file
  // thus libpng knows some bytes are missing
  png_set_sig_bytes(png_ptr, cmp_number);

  // read file info up to image data
  png_read_info(png_ptr, info_ptr);

  // get info of png image
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);
  int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
  int channels = png_get_channels(png_ptr, info_ptr);
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  TEXPACKR_LOG("[PNG] width = %d\n", width)
  TEXPACKR_LOG("[PNG] height = %d\n", height)
  TEXPACKR_LOG("[PNG] bit_depth = %d\n", bit_depth)
  switch (color_type)
  {
    case PNG_COLOR_TYPE_GRAY:
      TEXPACKR_LOG("[PNG] color type = 'PNG_COLOR_TYPE_GRAY' (bit depths 1, 2, 4, 8, 16)\n")
      break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      TEXPACKR_LOG("[PNG] color type = 'PNG_COLOR_TYPE_GRAY_ALPHA' (bit depths 8, 16)\n")
      break;
    case PNG_COLOR_TYPE_PALETTE:
      TEXPACKR_LOG("[PNG] color type = 'PNG_COLOR_TYPE_PALETTE' (bit depths 1, 2, 4, 8)\n")
      break;
    case PNG_COLOR_TYPE_RGB:
      TEXPACKR_LOG("[PNG] color type = 'PNG_COLOR_TYPE_RGB' (bit depths 8, 16)\n")
      break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
      TEXPACKR_LOG("[PNG] color type = 'PNG_COLOR_TYPE_RGB_ALPHA' (bit depths 8, 16)\n")
      break;
  }
  switch (interlace_type)
  {
    case PNG_INTERLACE_NONE:
      TEXPACKR_LOG("[PNG] interlace type = none\n")
      break;
    case PNG_INTERLACE_ADAM7:
      TEXPACKR_LOG("[PNG] interlace type = ADAM7\n")
      break;
  }
  switch (channels)
  {
    case 1:
      TEXPACKR_LOG("[PNG] channels = %d (GRAY, PALETTE)\n", channels)
      break;
    case 2:
      TEXPACKR_LOG("[PNG] channels = %d (GRAY_ALPHA)\n", channels)
      break;
    case 3:
      TEXPACKR_LOG("[PNG] channels = %d (RGB)\n", channels)
      break;
    case 4:
      TEXPACKR_LOG("[PNG] channels = %d (RGB_ALPHA or RGB + filter byte)\n", channels)
      break;
  }
  TEXPACKR_LOG("[PNG] rowbytes = %d\n", rowbytes)

  // allocate enough and continous memory space to whole entire image
  // note: i think we could allocate continous memory space that result in just png_bytep
  // but for some reason it might due to internal libpng's internal implementation that possibly
  // needs some flexibility in row by row pointer, thus we need to allocate memory space this way
  png_bytepp row_ptr = (png_bytepp)malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; ++y)
  {
    row_ptr[y] = (png_bytep)malloc(rowbytes);
  }
  // read image data
  png_read_image(png_ptr, row_ptr);

	// convert to 4 channels (RGBA) if channels is less than 4
	// GRAY
	if (channels == 1)
	{
		TEXPACKR_LOG("[PNG] image has %d channels, we need to convert to 4 channels\n", channels)		
		png_bytepp convt_row_ptr = texpackr_allocate_png_rgba_image_space(width, height);

		// copy from old image data to this new, copying byte value from source image to destination
		for (int y=0; y<height; ++y)
		{
			png_bytep dst_row = convt_row_ptr[y];
			png_bytep src_row = row_ptr[y];

			for (int x=0; x<width; ++x)
			{
				png_bytep dst_pixel = &dst_row[x*4];
				png_bytep src_pixel = &src_row[x];

				// same pixel value for all 3 components (RGB)
				png_byte pval = src_pixel[0];

				dst_pixel[0] = pval;
				dst_pixel[1] = pval;
				dst_pixel[2] = pval;
				dst_pixel[3] = 0xFF;
			}
		}
		
		// free source iamge data
		texpackr_free_png_image_data(row_ptr, height);
		// swap pointer for returning from this function
		row_ptr = convt_row_ptr;
	}
	// GRAY ALPHA
	else if (channels == 2)
	{
		TEXPACKR_LOG("[PNG] image has %d channels, we need to convert to 4 channels\n", channels)		
		png_bytepp convt_row_ptr = texpackr_allocate_png_rgba_image_space(width, height);

		// copy from old image data to this new, copying byte value from source image to destination
		for (int y=0; y<height; ++y)
		{
			png_bytep dst_row = convt_row_ptr[y];
			png_bytep src_row = row_ptr[y];

			for (int x=0; x<width; ++x)
			{
				png_bytep dst_pixel = &dst_row[x*4];
				png_bytep src_pixel = &src_row[x*2];

				// same pixel value for all 3 components (RGB)
				png_byte pval = src_pixel[0];

				dst_pixel[0] = pval;
				dst_pixel[1] = pval;
				dst_pixel[2] = pval;
				dst_pixel[3] = src_pixel[1];
			}
		}
		
		// free source iamge data
		texpackr_free_png_image_data(row_ptr, height);
		// swap pointer for returning from this function
		row_ptr = convt_row_ptr;
	}
	// RGB
	else if (channels == 3)
	{
		TEXPACKR_LOG("[PNG] image has %d channels, we need to convert to 4 channels\n", channels)
		png_bytepp convt_row_ptr = (png_bytepp)malloc(sizeof(png_bytep) * height);
		// 4 bytes per pixel
		int rowbs = width * 4;
		for (int y=0; y<height; ++y)
		{
			convt_row_ptr[y] = (png_bytep)malloc(rowbs);
		}

		// copy from old image data to this new, with 0xFF as alpha component
		for (int y=0; y<height; ++y)
		{
			png_bytep dst_row = convt_row_ptr[y];
			png_bytep src_row = row_ptr[y];

			for (int x=0; x<width; ++x)
			{
				png_bytep dst_pixel = &dst_row[x*4];
				png_bytep src_pixel = &src_row[x*3];
				dst_pixel[0] = src_pixel[0];
				dst_pixel[1] = src_pixel[1];
				dst_pixel[2] = src_pixel[2];
				dst_pixel[3] = 0xFF;
			}
		}
		
		// free source image data
		texpackr_free_png_image_data(row_ptr, height);
		// swap pointer for returning from this function
		row_ptr = convt_row_ptr;
	}

  // clear png resource
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

  // close file
  fclose(fp);
  fp = NULL;

  // return results
  if (rst_rowbytes != NULL)
  {
    *rst_rowbytes = rowbytes;
  }
  if (rst_width != NULL)
  {
    *rst_width = width;
  }
  if (rst_height != NULL)
  {
    *rst_height = height;
  }

  return row_ptr;
}

bool texpackr_write_png_file(const char* file_name, const png_bytepp data, int width, int height)
{ 
  // open file
  // note: libpng tells us to make sure we open in binary mode
  FILE *fp = fopen(file_name, "wb");
  if (fp == NULL)
		return false;
  
  // create png structure
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
  {
    // cannot create png structure
    TEXPACKR_ELOG("cannot create png structure\n")

    // close file
    fclose(fp);
    fp = NULL;
    return false;
  }

  // create png-info structure
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
  {
    TEXPACKR_ELOG("cannot create png info structure\n")

    // clear png resource
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    
    // close file
    fclose(fp);
    fp = NULL;
    return false;
  }

  // we need to set jump callback to handle error when we enter into a new routing before the call to png_*()
  // defined as convenient for future if you every call this in different routine
  // note: if use, need to call in routine that return bool type indicating the result of operation
#define PNG_WRITE_SETJMP(png_ptr, info_ptr, fp) \
  /* set jmp */  \
  if (setjmp(png_jmpbuf(png_ptr)))  \
  { \
    TEXPACKR_ELOG("error png's set jmp for write\n") \
                                              \
    /* clear png resource */                  \
    png_destroy_write_struct(&png_ptr, &info_ptr);   \
                                                                      \
    /* close file */ \
    fclose(fp);     \
    fp = NULL;      \
    return false;    \
  }

  // call this once as all relevant operations all happen in just one routine
  PNG_WRITE_SETJMP(png_ptr, info_ptr, fp)

  // set up input code
  png_init_io(png_ptr, fp);

  // set important parts of png_info first
  png_set_IHDR(png_ptr,
      info_ptr,
      width,
      height,
      8,
      PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT, 
      PNG_FILTER_TYPE_DEFAULT);
  // <... add any png_set_*() function in any order if need here

  // ready to write info we've set before actual image data
  png_write_info(png_ptr, info_ptr);

  // now it's time to write actual image data
  png_write_image(png_ptr, data);

  // done writing image
  // pass NULL as we don't need to write any set data i.e. comment
  png_write_end(png_ptr, NULL);

  // clear png resource
  png_destroy_write_struct(&png_ptr, &info_ptr);

  // close file
  fclose(fp);
  fp = NULL;

  return true;
}
