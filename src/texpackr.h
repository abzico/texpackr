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

	/// (internally used)
	/// opaque pointer, behind the scene it works with libpng's png_bytepp
	///
	/// sheet maintains this during creation phase
	/// to reduce memory usage and a need to hold each sprite's image data.
	/// Each sprite's image data will be put into this attribute each time
	/// it's inserted.
	///
	/// pixel format is RGBA 8-bit per channel.
	void* pixels;

} texpackr_sheet;

/// API for usage while building a sheet + meta file
/*
 * Create a new sheet.
 *
 * \param max_width maximum result sheet's width to pack sprites
 * \param max_height maximum result of sheet's height to pack sprites
 * \return newly created sheet
 */
extern texpackr_sheet* texpackr_sheet_new(int max_width, int max_height);

/*
 * Init with defaults and clean state of sheet's attributes and internal system.
 *
 * \param s sheet
 * \param max_width sheet's maximum width
 * \param max_height sheets's maximum height
 */
extern void texpackr_sheet_init_defaults(texpackr_sheet* s, int max_width, int max_height);

/*
 * Free sheet.
 *
 * \param s sheet
 */
extern void texpackr_sheet_free(texpackr_sheet* s);

/*
 * Free internals of sheet
 *
 * Use this to free internal of sheet, so user can do something else on top without actually free it entire memory space.
 *
 * \param s sheet
 */
extern void texpackr_sheet_free_internals(texpackr_sheet* s);

/*
 * Batch insert input images.
 *
 * \param s sheet
 * \param img_filenames pointer to array of null-terminated string of image filenames
 * \param image_count number of image sent in as input
 * \return true if success, otherwise false.
 */
extern bool texpackr_sheet_batch_imgs(texpackr_sheet* s, const char** img_filenames, unsigned int image_count);

/*
 * Insert a new image into the sheet.
 *
 * \param s sheet
 * \param image_filename image filename to insert into the sheet
 * \return true if success, otherwise false.
 */
extern bool texpackr_sheet_insert_img(texpackr_sheet* s, const char* image_filename);

/*
 * Get sprite's image data from its associated image filename.
 *
 * \param s sheet
 * \param image_filename image filename to get image data from
 * \return image's pixel data in 2 dimensional array of rows of image data.
 */
extern unsigned char** texpackr_sheet_get_sprite_image_data(texpackr_sheet* s, const char* image_filename);

/*
 * Export sheet into image sheet, and meta file.
 *
 * \param s sheet
 * \param sheet_filename output sheet's image filename to write into
 * \param meta_filenam output sheet's meta filename to write into
 * \return true if success, otherwise false.
 */
extern bool texpackr_sheet_export(texpackr_sheet* s, const char* sheet_filename, const char* meta_filename);

/*
 * Clear sheet and start it over.
 * After this call, its sprites, internal states, and other attributs will be clear
 * and reset back to the initial state.
 *
 * It's equivalent to the call texpackr_sheet_free_internals() then follows by texpackr_init_defaults().
 */
extern void texpackr_sheet_clear(texpackr_sheet* s);

/// API for usage when sheet + meta file already exported

/*
 * Extract sprite image data from sprite image.
 * Use this when you're outside of creation phase, and only have access to sheet image and its meta file. This will have better memory usage than trying to access it during cration phase.
 *
 * \param sheetimage_filename sheet image filename
 * \param sheet_meta_filename sheet meta filename
 * \return image's pixel data in 2 dimensional array of rows of image data.
 */
extern unsigned char** texpackr_extract_sprite_image_data(const char* sheetimage_filename, const char* sheet_meta_filename);
// TODO: Return as hashmap, or actual list of texpackr_sprite

#endif
