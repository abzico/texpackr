#ifndef TEXPACKR_PNG_UTIL_H_
#define TEXPACKR_PNG_UTIL_H_

#include <png.h>
#include <stdbool.h>

/*
 * Free png image data.
 *
 * \param data image data
 * \param height height of the image.
 */
extern void texpackr_free_png_image_data(png_bytepp data, int height);

/*
 * Read png file.
 *
 * \param file_name target file name to read from
 * \param rst_rowbytes to be filled in for result of rowbytes of image
 * \param rst_width to be filled in for result of image's width
 * \param rst_height to be filled in for result of image's height
 * \return image data as `png_bytepp`.
 */
extern png_bytepp texpackr_read_png_file(const char* file_name, int* rst_rowbytes, int* rst_width, int* rst_height);

/*
 * Write into png file.
 *
 * \param file_name file name to write png file into
 * \param data input image data used as input to write into result image file
 * \param width width of image file
 * \param height height of image file
 * \return return true if operation is successful, otherwise false is returned.
 */
extern bool texpackr_write_png_file(const char* file_name, const png_bytepp data, int width, int height);

#endif
