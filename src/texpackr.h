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
	/// image data
	/// availability depends on whether query with which function
	/// it can contain NULL, or actual image data.
	/// This is due to keeping memory usage as low as possible.
	void* image_data;

} texpackr_sprite;

struct texpackr_node;

/// sheet as top level to hold other information
typedef struct 
{
	/// list of sprites in the sheet
	texpackr_sprite* sprites;

	/// number of sprite in the sheet
	int sprite_count;

	/// (internally used)
	/// managed sprite count, available mem space for adding more sprites
	int msprite_count;

	/// resolution or image size of the sheet
	texpackr_vec2 size;

	/// (internally used)
	struct texpackr_node* node;

} texpackr_sheet;

/// API for usage while building a sheet + meta file
extern texpackr_sheet* texpackr_sheet_new(int max_width, int max_height);
extern void texpackr_sheet_init_defaults(texpackr_sheet* s, int max_width, int max_height);
extern void texpackr_sheet_free(texpackr_sheet* s);
extern void texpackr_sheet_free_internals(texpackr_sheet* s);
extern bool texpackr_sheet_batch_imgs(texpackr_sheet* s, const char** img_filenames, unsigned int image_count);
extern bool texpackr_sheet_insert_img(texpackr_sheet* s, const char* image_filename);
extern unsigned char** texpackr_sheet_get_sprite_image_data(texpackr_sheet* s, const char* image_filename);
extern bool texpackr_sheet_export(texpackr_sheet* s, const char* sheet_filename, const char* meta_filename);
extern void texpackr_sheet_clear(texpackr_sheet* s);

/// API for usage when sheet + meta file already exported
extern unsigned char** texpackr_extract_sprite_image_data(const char* sheetimage_filename, const char* sheet_meta_filename);
// TODO: Return as hashmap, or actual list of texpackr_sprite

#endif
