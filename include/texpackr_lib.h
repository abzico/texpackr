#ifndef TEXPACKR_LIB_H_
#define TEXPACKR_LIB_H_

#include <stdbool.h>

///
/// Pack input images and write into output .tga image file.
///
/// \param filepath filepath of .tga file to write into
/// \param images pointer to array of image file path as input to packing operation
/// \param imageds_count count of input images' filepath
/// \return true if operation is successful, otherwise false for failure
///
extern bool TEXPACKR_pack_as_tga(const char* filepath, const char** image_paths, unsigned int images_count);

#endif
