#include "texpackr.h"
#include <png.h>
#include <stdlib.h>

/*
 * define debugging information to spit out from libpng
 * higher number, more info will be given out
 * valid value is 0 to 3
 */
#define PNG_DEBUG 3

/*
 * rectangle to represent area of each node
 */
typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
} texpackr_rect;

/*
 * node used in packing algorithm
 */
typedef struct texpackr_node texpackr_node;
struct texpackr_node
{
	/// it can spawn new node either to left or right
	texpackr_node* child[2];

	/// rectangle representing area of a node
	texpackr_rect rc;

	/// use to refer to the image
	/// in which finally expose filename for user to access in the game code
	/// if it's -1 (which is by default) there's no image associated with this node yet
	int image_id;
};

texpackr_sheet* texpackr_sheet_new()
{
	texpackr_sheet* out = malloc(sizeof(texpackr_sheet));

	return out;
}

void texpackr_sheet_free_internals(texpackr_sheet* s)
{
	for (int i=0; i<s->sprite_count; ++i)
	{
		texpackr_sprite* sprite = (s->sprites + i);

		// free string
		free(sprite->filename);
		sprite->filename = NULL;
	}

	s->sprite_count = 0;

	// free list of sprites
	free(s->sprites);
	s->sprites = NULL;
}

void texpackr_sheet_free(texpackr_sheet* s)
{
	texpackr_sheet_free_internals(s);
	free(s);
}

void texpackr_sheet_batch_imgs(texpackr_sheet* s, const char** img_filenames, unsigned int image_count)
{

}

void texpackr_sheet_insert_img(texpackr_sheet* s, const char* image_filename)
{

}

void texpackr_sheet_pack(texpackr_sheet* s)
{

}

void texpackr_sheet_export(texpackr_sheet* s, const char* sheet_filename, const char* meta_filename)
{

}

void texpackr_sheet_clear(texpackr_sheet* s)
{

}
