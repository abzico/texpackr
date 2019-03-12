#include "texpackr.h"
#include "texpackr_internal.h"
#include "png_util.h"
#include <stdlib.h>

/// refer to functions defined in treetrv.c
extern void texpackr_free_all_nodes(texpackr_node* n);

#define RECT_SET(ptr,l,t,r,b) \
	(*ptr).left = l;						\
	(*ptr).top = t;							\
	(*ptr).right = r;						\
	(*ptr).bottom = b;

texpackr_sheet* texpackr_sheet_new()
{
	texpackr_sheet* out = malloc(sizeof(texpackr_sheet));
	texpackr_sheet_init_defaults(out);
	return out;
}

void texpackr_sheet_init_defaults(texpackr_sheet* s)
{
	s->size.x = 0;
	s->size.y = 0;

	s->sprite_count = 0;

	s->sprites = NULL;

	// create root node
	texpackr_node* node = NULL;

	node = malloc(sizeof(texpackr_node));
	node->child[0] = NULL;
	node->child[1] = NULL;
	RECT_SET(&node->rc, -1, -1, -1, -1)
	node->image_id = -1;

	// set root node to sheet
	s->node = node;
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

	// free list of sprites
	free(s->sprites);
	s->sprites = NULL;

	// traverse to free all nodes
	texpackr_free_all_nodes(s->node);

	s->sprite_count = 0;
}

void texpackr_sheet_free(texpackr_sheet* s)
{
	texpackr_sheet_free_internals(s);
	free(s);
}

bool texpackr_sheet_batch_imgs(texpackr_sheet* s, const char** img_filenames, unsigned int image_count)
{
	return true;
}

bool texpackr_sheet_insert_img(texpackr_sheet* s, const char* image_filename)
{
	// if we're not at the leaf node
	
	return true;
}

bool texpackr_sheet_pack(texpackr_sheet* s, unsigned int max_width, unsigned int max_height)
{
	return true;
}

bool texpackr_sheet_export(texpackr_sheet* s, const char* sheet_filename, const char* meta_filename)
{
	return true;
}

void texpackr_sheet_clear(texpackr_sheet* s)
{

}
