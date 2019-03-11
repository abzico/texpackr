#ifndef TEXPACKR_H_
#define TEXPACKR_H_

#include <stdbool.h>

/// vector2
/// no need to be float, integer type is enough for us
typedef struct
{
	int x;
	int y;
} texpackr_vec2;

/// sprite struct
/// important attribute is offset, and size
typedef struct
{
	/// dynamically allocate as exact number of bytes need to hold
	/// path string
	char* filename;

	/// offset of sprite in the atlas
	texpackr_vec2 offset;
	/// size of sprite in the atlas
	texpackr_vec2 size;

} texpackr_sprite;

/// sheet as top level to hold other information
typedef struct 
{
	/// list of sprites in the sheet
	texpackr_sprite* sprites;

	/// number of sprite in the sheet
	int sprite_count;

	/// resolution or image size of the sheet
	texpackr_vec2 size;

} texpackr_sheet;

extern texpackr_sheet* texpackr_sheet_new();

extern void texpackr_sheet_free_internals(texpackr_sheet* s);

extern void texpackr_sheet_free(texpackr_sheet* s);

extern void texpackr_sheet_batch_imgs(texpackr_sheet* s, const char** img_filenames, unsigned int image_count);
extern void texpackr_sheet_insert_img(texpackr_sheet* s, const char* image_filename);

extern void texpackr_sheet_pack(texpackr_sheet* s);

extern void texpackr_sheet_export(texpackr_sheet* s, const char* sheet_filename, const char* meta_filename);

extern void texpackr_sheet_clear(texpackr_sheet* s);

#endif
