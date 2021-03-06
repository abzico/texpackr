#include "texpackr/texpackr.h"
#include "texpackr/internal.h"
#include "texpackr/log.h"
#include "texpackr/png_util.h"
#include <stdlib.h>
#include <string.h>

/// refer to functions defined in treetrv.c
extern void texpackr_free_all_nodes(texpackr_node* n);

// functs signature
static texpackr_node* insert_img(texpackr_node* n, int img_width, int img_height, int img_rowbytes, png_bytepp img_data);

#define RECT_SET(ptr,l,t,r,b) \
	(*ptr).left = l;						\
	(*ptr).top = t;							\
	(*ptr).right = r;						\
	(*ptr).bottom = b;

texpackr_sheet* texpackr_sheet_new(int max_width, int max_height)
{
	texpackr_sheet* out = malloc(sizeof(texpackr_sheet));
	texpackr_sheet_init_defaults(out, max_width, max_height);
	return out;
}

void texpackr_sheet_init_defaults(texpackr_sheet* s, int max_width, int max_height)
{
	s->size.x = max_width;
	s->size.y = max_height;
	// create enough memory space to hold png image data
	// note: save as void* to hide the implementation detail from users,
	// otherwise we need to include png.h in header file which is not nice!
	s->pixels = (void*)texpackr_allocate_png_rgba_image_space(max_width, max_height);

	s->sprite_count = 0;
	s->sprites = NULL;

	// create root node
	texpackr_node* node = NULL;

	node = malloc(sizeof(texpackr_node));
	node->child[0] = NULL;
	node->child[1] = NULL;

	RECT_SET(&node->rc, 0, 0, max_width, max_height)
	node->image_id = -1;

	// set root node to sheet
	s->node = node;
}

void texpackr_sheet_free_internals(texpackr_sheet* s)
{
	for (int i=0; i<s->sprite_count; ++i)
	{
		texpackr_sprite* sprite = (s->sprites + i);
		texpackr_sprite_free_internals(sprite);
	}

	// free list of sprites
	free(s->sprites);
	s->sprites = NULL;

	// free its image data
	if (s->pixels != NULL)
	{
		texpackr_free_png_image_data((png_bytepp)s->pixels, s->size.y);
		s->pixels = NULL;
	}

	// traverse to free all nodes
	texpackr_free_all_nodes(s->node);
	s->node = NULL;

	s->sprite_count = 0;
	s->msprite_count = 0;
	s->size.x = -1;
	s->size.y = -1;
}

void texpackr_sheet_free(texpackr_sheet* s)
{
	texpackr_sheet_free_internals(s);
	free(s);
}

bool texpackr_sheet_batch_imgs(texpackr_sheet* s, const char** img_filenames, unsigned int image_count)
{
	// loop through all of input image filenames
	for (int i=0; i<image_count; ++i)
	{
		const char* filename = *(img_filenames + i);

		bool result = texpackr_sheet_insert_img(s, filename);
		// return immediately if failed
		if (!result)
		{
			TEXPACKR_ELOG("[BATCH INSERT] failed in inserting %s\n", filename)
			return false;
		}
		else
		{
			TEXPACKR_LOG("[BATCH INSERT] succeeded inserting %s\n", filename)
		}
	}
	return true;
}

bool texpackr_sheet_insert_img(texpackr_sheet* s, const char* image_filename)
{
	// image's meta data
	int img_width = 0;
	int img_height = 0;
	int img_rowbytes = 0;

	TEXPACKR_LOG("[INSERT] inserting %s\n", image_filename)

	// read image to get its meta info and image data
	png_bytepp image_data = texpackr_read_png_file(image_filename, &img_rowbytes, &img_width, &img_height);

	// try to insert image
	texpackr_node* node = insert_img(s->node, img_width, img_height, img_rowbytes, image_data);
	if (node != NULL)
	{

		// insert a new sprite data for later query and one-time writing into the file
		if (s->sprites == NULL)
		{
			// create sprites mem space if needed
			texpackr_sprite* sprite_ptr = malloc(sizeof(texpackr_sprite));
			sprite_ptr->filename = NULL;
			sprite_ptr->image_data = NULL;

			s->sprites = sprite_ptr;
			s->msprite_count = 1;
		}
		
		// if need to expand memory space for more sprites to be added
		if (s->sprite_count >= s->msprite_count)
		{
			const int increment_size = 1;
			texpackr_sprite* sprites_ptr = realloc(s->sprites, sizeof(texpackr_sprite) * (s->msprite_count+increment_size));

			if (sprites_ptr == NULL)
			{
				// no need to clear resource here as original pointer is left intact
				// after call to realloc()
				return false;
			}
			else
			{
				// set back pointer
				s->sprites = sprites_ptr;
				// clear memro
				memset(s->sprites + s->sprite_count, 0, sizeof(texpackr_sprite) * increment_size);
				// update managed sprite count
				s->msprite_count++;
			}
		}

		// get size of sheet
		texpackr_vec2 s_size = s->size;

		// set image's meta data into sprite
		texpackr_sprite* sprite_ptr = s->sprites + s->sprite_count;
		// - filename
		sprite_ptr->filename = calloc(1, sizeof(char) * (strlen(image_filename) + 1));
		strncpy(sprite_ptr->filename, image_filename, strlen(image_filename));
		// - offset
		sprite_ptr->offset.x = node->rc.left;
		sprite_ptr->offset.y = node->rc.top;
		// - size
		sprite_ptr->size.x = img_width;
		sprite_ptr->size.y = img_height;
		// - texcoord_u & texcoord_v
		sprite_ptr->texcoord_u.x = sprite_ptr->offset.x * 1.0f / s_size.x;
		sprite_ptr->texcoord_u.y = (sprite_ptr->offset.x + sprite_ptr->size.x) * 1.0f / s_size.x;
		sprite_ptr->texcoord_v.x = sprite_ptr->offset.y * 1.0f / s_size.y;
		sprite_ptr->texcoord_v.y = (sprite_ptr->offset.y + sprite_ptr->size.y) * 1.0f / s_size.y;
		// - image data
		// TODO: Fix to remove image_data entirely maybe? we don't really use it across the board...
		sprite_ptr->image_data = NULL;

		// update sprite count
		s->sprite_count++;

		// image id is the index+1 to represent it
		node->image_id = s->sprite_count;

		// get pointer to maintained pixels of sheet
		png_bytepp maintained_pixels = (png_bytepp)s->pixels;

		int offset_x = sprite_ptr->offset.x;
		int offset_y = sprite_ptr->offset.y;

		// make sure input image is smaller than our sheet image
		// note: ignore checking here
		for (int y=0; y < img_height; ++y)
		{
			png_bytep src_row_ptr = image_data[y];
			png_bytep dst_row_ptr = maintained_pixels[y+offset_y];

			for (int x=0; x < img_width; ++x)
			{
				png_bytep src_pixel = &src_row_ptr[x*4];
				png_bytep dst_pixel = &dst_row_ptr[(x+offset_x)*4];

				dst_pixel[0] = src_pixel[0];
				dst_pixel[1] = src_pixel[1];
				dst_pixel[2] = src_pixel[2];
				dst_pixel[3] = src_pixel[3];
			}
		}

		// free image data
		texpackr_free_png_image_data(image_data, img_height);

		TEXPACKR_LOG("[INSERT] insert %s succeeded image_id: %d\n", image_filename, node->image_id)

		return true;
	}
	else
	{
		TEXPACKR_ELOG("[INSERT] insert %s failed\n", image_filename)
		// free image data
		texpackr_free_png_image_data(image_data, img_height);
		return false;
	}
}

texpackr_node* insert_img(texpackr_node* n, int img_width, int img_height, int img_rowbytes, png_bytepp img_data)
{
	// not at the leaf node
	if (n->child[0] != NULL &&
			n->child[1] != NULL)
	{
		TEXPACKR_LOG("[INSERT] not at leaf node\n")
		// try inserting into first child
		texpackr_node* new_node = insert_img(n->child[0], img_width, img_height, img_rowbytes, img_data);
		if (new_node != NULL)
			return new_node;

		// no room, insert into second child
		return insert_img(n->child[1], img_width, img_height, img_rowbytes, img_data);
	}
	else
	{
		TEXPACKR_LOG("[INSERT] at leaf node\n")

		// if there's already image here, then return
		// note: -1 means empty, not set yet
		if (n->image_id != -1)
		{
			TEXPACKR_LOG("[INSERT] return NULL, image_id != -1\n")
			return NULL;
		}

		// get rect area
		texpackr_rect rc = n->rc;
		int rc_width = rc.right - rc.left + 1;
		int rc_height = rc.bottom - rc.top + 1;
		// if image doesn't fit into node's area, then return
		if (img_width > rc_width ||
				img_height > rc_height)
		{
			TEXPACKR_LOG("[INSERT] return NULL, image doens't fit in this node's area\n")
			return NULL;
		}

		// if it perfectly fit, then accept
		if (img_width == rc_width &&
				img_height == rc_height)
		{
			TEXPACKR_LOG("[INSERT] return node, perfectly fit\n")
			return n;
		}

		// otherwise split this node
		texpackr_node* left_node = malloc(sizeof(texpackr_node));
		RECT_SET(&(left_node->rc), -1, -1, -1, -1)
		left_node->image_id = -1;
		left_node->child[0] = NULL;
		left_node->child[1] = NULL;
		n->child[0] = left_node;

		texpackr_node* right_node = malloc(sizeof(texpackr_node));
		RECT_SET(&(right_node->rc), -1, -1, -1, -1)
		right_node->image_id = -1;
		right_node->child[0] = NULL;
		right_node->child[1] = NULL;
		n->child[1] = right_node;

		// decide which way to split
		int dw = rc_width - img_width;
		int dh = rc_height - img_height;
		
		// temporary rect
		texpackr_rect rect;

		TEXPACKR_LOG("[INSERT] dw = %d, dh = %d\n", dw, dh)

		if (dw > dh)
		{
			TEXPACKR_LOG("[INSERT] split along the width\n")
			
			rect.left = rc.left;
			rect.top = rc.top;
			rect.right = rc.left + img_width - 1;
			rect.bottom = rc.bottom;
			memcpy(&(n->child[0]->rc), &rect, sizeof(texpackr_rect));
			TEXPACKR_LOG("[INSERT] split rect: %d %d %d %d\n", n->child[0]->rc.left, n->child[0]->rc.top, n->child[0]->rc.right, n->child[0]->rc.bottom)

			rect.left = rc.left + img_width;
			rect.top = rc.top;
			rect.right = rc.right;
			rect.bottom = rc.bottom;
			memcpy(&(n->child[1]->rc), &rect, sizeof(texpackr_rect));
			TEXPACKR_LOG("[INSERT] split rect: %d %d %d %d\n", n->child[1]->rc.left, n->child[1]->rc.top, n->child[1]->rc.right, n->child[1]->rc.bottom)
		}
		else
		{
			TEXPACKR_LOG("[INSERT] split along the height\n")

			rect.left = rc.left;
			rect.top = rc.top;
			rect.right = rc.right;
			rect.bottom = rc.top + img_height - 1;
			memcpy(&(n->child[0]->rc), &rect, sizeof(texpackr_rect));
			TEXPACKR_LOG("[INSERT] split rect: %d %d %d %d\n", n->child[0]->rc.left, n->child[0]->rc.top, n->child[0]->rc.right, n->child[0]->rc.bottom)

			rect.left = rc.left;
			rect.top = rc.top + img_height;
			rect.right = rc.right;
			rect.bottom = rc.bottom;
			memcpy(&(n->child[1]->rc), &rect, sizeof(texpackr_rect));
			TEXPACKR_LOG("[INSERT] split rect: %d %d %d %d\n", n->child[1]->rc.left, n->child[1]->rc.top, n->child[1]->rc.right, n->child[1]->rc.bottom)
		}

		// insert into first child we created
		return insert_img(n->child[0], img_width, img_height, img_rowbytes, img_data);
	}
}

void texpackr_sheet_clear(texpackr_sheet* s)
{
	// save size of sheet
	// as these info will be lost after we call texpackr_sheet_free_internals()
	texpackr_vec2 size = s->size;
	
	// just call free internals
	texpackr_sheet_free_internals(s);

	// now init defaults again
	texpackr_sheet_init_defaults(s, size.x, size.y);
}

unsigned char* texpackr_sheet_get_sprite_image_data(texpackr_sheet* s, const char* image_filename)
{
	return NULL;
}

bool texpackr_sheet_export(texpackr_sheet* s, const char* sheet_filename, const char* meta_filename)
{
	// write into sheet image
	bool result = texpackr_write_png_file(sheet_filename, s->pixels, s->size.x, s->size.y);
	if (!result)
	{
		TEXPACKR_ELOG("[EXPORT] Error exporting sheet image file %s\n", sheet_filename)
		return false;
	}

	// write into meta file
	result = texpackr_sheetmeta_write_meta_file(meta_filename, s);
	if (!result)
	{
		TEXPACKR_ELOG("[EXPORT] Error exporting meta file %s\n", meta_filename)
		return false;
	}
	
	return true;
}

unsigned char* texpackr_extract_sprite_image_data(const char* sheetimage_filename, const char* sheet_meta_filename)
{
	return NULL;
}
