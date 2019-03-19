#include "texpackr/meta.h"
#include "texpackr/log.h"
#include "texpackr/sprite.h"
#include "texpackr/png_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// equivalent to fgets() but works on string instead of file
/*
 * Equivalent to fgets() but operate on string instead of file.
 * It will read string upto `dst_size` or stop reading whenever find newline character,
 * then return string so far as read via `dst_str`.
 *
 * `offset` will be updated according to the current number of bytes read.
 * First call should always set it to 0, then consecutive call will be automatically updated by
 * this function, just re-supply it in the function call.
 *
 * At last, it will stop whenever it has read all of the bytes from the `input_str`.
 *
 * \param dst_str destination string as output
 * \param dst_size up to number of size to read for destination string
 * \param input_str input string to read
 * \param offset pointer to offset number for consecutive call
 * \return string as read starting from `offset` and up to `dst_size`. It returns NULL when all of `input_str`'s bytes has been read.
 */
static char* sgets(char* dst_str, int dst_size, const char* input_str, int* offset)
{
	int size_count = 0;

	const int of = *offset;

	if (of > strlen(input_str))
		return NULL;

	for (int i = 0; i<dst_size; ++i)
	{
		char c = input_str[of + i];

		if (c == '\n')
		{
			dst_str[i] = c;
			dst_str[i+1] = '\0';
			++size_count;
			break;
		}
		else if (c == '\0')
		{
			dst_str[i] = c;
			++size_count;
			break;
		}
		else
		{
			dst_str[i] = c;
			++size_count;
		}
	}

	// set offset back for future calls
	*offset = *offset + size_count;

	return dst_str;
}

/// free function for individual element setting for hashmap's custom element freeing memory space
/// note: this function only responsible for freeing element's internals not element itself as
/// it's managed by hashmapc
static void free_internals_texpackr_sprite_for_hashmapc(void* ptr)
{
	// convert to texpackr_sprite
	texpackr_sprite* spr = (texpackr_sprite*)ptr;

	TEXPACKR_LOG("free sprite for hashmap: %s offset: %d,%d size: %d,%d image_data == NULL?: %d\n", spr->filename, spr->offset.x, spr->offset.y, spr->size.x, spr->size.y, spr->image_data == NULL ? 1 : 0)

	// free internals (not itself)
	texpackr_sprite_free_internals(spr);
}

texpackr_sheetmeta* texpackr_parse(const char* sheet_meta_filename)
{
	FILE* fp = fopen(sheet_meta_filename, "rb");
	if (fp == NULL)
	{
		TEXPACKR_ELOG("[META] Error opening file for reading %s\n", sheet_meta_filename)
		return NULL;
	}

	// get total bytes of the file
	// we're going to read whole file ignoring comment lines
	// thus we need a buffer to hold no-comment lines
	int err = fseek(fp, 0L, SEEK_END);
	if (err != 0)
	{
		TEXPACKR_ELOG("[META] error setting file position %s\n", sheet_meta_filename)
		fclose(fp);
		fp = NULL;
		return NULL;
	}

	long file_size = ftell(fp);
	if (file_size == -1)
	{
		TEXPACKR_ELOG("[META] error getting total file size %s\n", sheet_meta_filename)
		fclose(fp);
		fp = NULL;
		return NULL;
	}

	// set file position back to beginning
	err = fseek(fp, 0L, SEEK_SET);
	if (err != 0)
	{
		TEXPACKR_ELOG("[META] error setting file position at beginning %s\n", sheet_meta_filename)
		fclose(fp);
		fp = NULL;
		return NULL;
	}

	// line buffer to read line by line from the file
	char line[256];
	// define no-comment-lines buffer dynamically to avoid stack size limitation of 2MB
	// yep, in case the input file size is larger than 2MB
	// 
	// the size allocated might be larger than need as we didn't pre-calculate size without comment lines
	// before reading which might need more effort, we keep it simple
	char* no_comment_lines = calloc(1, sizeof(char) * file_size);

	while (fgets(line, sizeof(line), fp))
	{
		// ignore comment lines
		if (strncmp(line, "#", 1) != 0)
		{
			// append read string into no-comment-lines buffer
			strncat(no_comment_lines, line, strlen(line));
		}
	}

	// close file
	fclose(fp);
	fp = NULL;

	// create meta struct to hold read information
	texpackr_sheetmeta* sm = texpackr_sheetmeta_new();
	
	// operate like fgets() but on string with sgets()
	int offset = 0;	// start at offset 0 always
	// read width + height
	sgets(line, sizeof(line), no_comment_lines, &offset);
	if (sscanf(line, "%d %d", &sm->size.x, &sm->size.y) != 2)
	{
		TEXPACKR_ELOG("[META] error width/height of the sheet from meta file %s\n", sheet_meta_filename)
		// free meta structure
		texpackr_sheetmeta_free(sm);
		sm = NULL;

		// continue further at the end of function to return sm as NULL
	}
	
	int sprite_count = 0;
	// read sprite count
	sgets(line, sizeof(line), no_comment_lines, &offset);
	if (sscanf(line, "%d", &sprite_count) != 1)
	{
		TEXPACKR_ELOG("[META] error sprite count inside sheet from meta file %s\n", sheet_meta_filename)
		// free meta structure
		texpackr_sheetmeta_free(sm);
		sm = NULL;

		// continue further at the end of function to return sm as NULL
	}

	// temporary string to read in sprite's filename
	char sprite_filename[255];
	// read sprites for number of times we have known prior to this point
	for (int i=0; i<sprite_count; ++i)
	{
		texpackr_vec2 read_offset;
		texpackr_vec2 read_size;
		texpackr_vec2f read_texcoord_u;
		texpackr_vec2f read_texcoord_v;

		// proceed through next chunk of string
		char* chk = sgets(line, sizeof(line), no_comment_lines, &offset);
		if (chk == NULL)
		{
			// should not happen
			TEXPACKR_ELOG("[META] error reading sprite's meta info line from %s (premature error)\n", sheet_meta_filename)

			// free meta structure (included all successfully added item up to this point)
			texpackr_sheetmeta_free(sm);
			sm = NULL;

			break;
		}

		if (sscanf(line, "%s %d %d %d %d %f %f %f %f", sprite_filename, &read_offset.x, &read_offset.y, &read_size.x, &read_size.y, &read_texcoord_u.x, &read_texcoord_u.y, &read_texcoord_v.x, &read_texcoord_v.y) == 9)
		{
			// create a new texpackr_sprite struct to add into hashmap
			texpackr_sprite* sprite = malloc(sizeof(texpackr_sprite));

			// allocate exact space to hold filename for sprite
			sprite->filename = malloc(sizeof(char) * (strlen(sprite_filename)+1));
			strncpy(sprite->filename, sprite_filename, strlen(sprite_filename));

			// set read attribute
			sprite->offset.x = read_offset.x;
			sprite->offset.y = read_offset.y;
			sprite->size.x = read_size.x;
			sprite->size.y = read_size.y;
			sprite->texcoord_u.x = read_texcoord_u.x;
			sprite->texcoord_u.y = read_texcoord_u.y;
			sprite->texcoord_v.x = read_texcoord_v.x;
			sprite->texcoord_v.y = read_texcoord_v.y;

			// no image data
			sprite->image_data = NULL;

			// add this item into hashmap using its filename as the key
			hashmapc_insert(sm->sprites, sprite_filename, sprite);

			TEXPACKR_LOG("[META] found sprite %s offset: %d,%d size: %d,%d, texcoord_u: %f,%f, texcoord_v: %f,%f\n", sprite->filename, sprite->offset.x, sprite->offset.y, sprite->size.x, sprite->size.y, sprite->texcoord_u.x, sprite->texcoord_u.y, sprite->texcoord_v.x, sprite->texcoord_v.y)
		}
		else
		{
			TEXPACKR_ELOG("[META] error reading sprite's meta info line from %s\n", sheet_meta_filename)

			// free meta structure (included all successfully added item up to this point)
			texpackr_sheetmeta_free(sm);
			sm = NULL;

			break;
		}
	}

	// free string buffer
	free(no_comment_lines);
	no_comment_lines = NULL;

	return sm;
}

texpackr_sheetmeta* texpackr_sheetmeta_new()
{
	texpackr_sheetmeta* out = malloc(sizeof(texpackr_sheetmeta));

	out->size.x = -1;
	out->size.y = -1;
	out->sprites = hashmapc_new(sizeof(texpackr_sprite));
	hashmapc_set_free_internals_elem_func(out->sprites, free_internals_texpackr_sprite_for_hashmapc);

	return out;
}

void texpackr_sheetmeta_free(texpackr_sheetmeta* meta)
{
	// free internal hashmap
	hashmapc_free(meta->sprites);
	meta->sprites = NULL;

	// free itself
	free(meta);
}

bool texpackr_sheetmeta_write_meta_file(const char* sheet_meta_filename, const texpackr_sheet* s)
{
	// open file
	FILE* fp = fopen(sheet_meta_filename, "wb");
	if (fp == NULL)
	{
		TEXPACKR_ELOG("[META] Error trying to open file for writing %s\n", sheet_meta_filename)
		return false;
	}

	// get size of sheet
	texpackr_vec2 s_size = s->size;

	// write comment at the top
	fprintf(fp, "# generated by texpackr (https://github.com/abzico/texpackr)\n");
	// write sheet's width and height
	fprintf(fp, "%d %d\n", s_size.x, s_size.y);
	// write number of sprites
	int sprite_count = s->sprite_count;
	fprintf(fp, "%d\n", sprite_count);

	float texcoord_min_u, texcoord_max_u, texcoord_min_v, texcoord_max_v;

	// write sprite info for number of sprites read
	for (int i=0; i<sprite_count; ++i)
	{
		// get sprite
		texpackr_sprite* spr = s->sprites + i;
		texpackr_vec2 offset = spr->offset;
		texpackr_vec2 size = spr->size;
		texcoord_min_u = offset.x * 1.0f / s_size.x;
		texcoord_max_u = (offset.x + size.x) * 1.0f / s_size.x;
		texcoord_min_v = offset.y * 1.0f / s_size.y;
		texcoord_max_v = (offset.y + size.y) * 1.0f / s_size.y;
		fprintf(fp, "%s %d %d %d %d %f %f %f %f\n", spr->filename, offset.x, offset.y, size.x, size.y,
				texcoord_min_u, texcoord_max_u, texcoord_min_v, texcoord_max_v);
	}

	// close file
	fclose(fp);
	fp = NULL;

	return true;
}
