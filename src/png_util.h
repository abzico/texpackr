#ifndef TEXPACKR_PNG_UTIL_H_
#define TEXPACKR_PNG_UTIL_H_

#include <png.h>
#include <stdbool.h>

extern void texpackr_free_image_data(png_bytepp data, int height);
extern png_bytepp texpackr_read_png_file(const char* file_name, int* rst_rowbytes, int* rst_width, int* rst_height);
extern bool texpackr_write_png_file(const char* file_name, const png_bytepp data, int width, int height);

#endif
