// test program for capability of texpackr
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "test_common.h"

#define SHEET_MAX_WIDTH 1024
#define SHEET_MAX_HEIGHT 1024

static void print_all_sprites(const texpackr_sheet* s)
{
	if (s->sprite_count == 0)
	{
		printf("there is no sprites in the sheet\n");
	}
	else
	{
		for (int i=0; i<s->sprite_count; i++)
		{
			texpackr_sprite* sprite_ptr = s->sprites + i;
			printf("sprite[%d] filename: %s, offset: %d,%d, size: %d,%d, texcoord_u: %f,%f, texcoord_v: %f,%f\n", i, sprite_ptr->filename, sprite_ptr->offset.x, sprite_ptr->offset.y, sprite_ptr->size.x, sprite_ptr->size.y, sprite_ptr->texcoord_u.x, sprite_ptr->texcoord_u.y, sprite_ptr->texcoord_v.x, sprite_ptr->texcoord_v.y);
		}
	}
}

static void print_sprite(const texpackr_sprite* sp)
{
	printf("sprite filename: %s, offset: %d,%d, size: %d,%d, texcoord_u: %f,%f, texcoord_v: %f,%f\n", sp->filename, sp->offset.x, sp->offset.y, sp->size.x, sp->size.y, sp->texcoord_u.x, sp->texcoord_u.y, sp->texcoord_v.x, sp->texcoord_v.y);
}

static void print_sheetmeta(texpackr_sheetmeta* meta)
{
	printf("meta info: size: %d,%d , sprite count in hashmap: %d\n", meta->size.x, meta->size.y, meta->sprites->size);

	// (known key)
	// printing known keys out, most likely we will known the key before hand so...
	const texpackr_sprite* sp = NULL;
	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/1.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/2.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/3.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/4.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/5.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/6.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/7.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "assets/8.png");
	if (sp == NULL)
		printf("Not found sprite with key 'assets/8.png'\n");
}

static void cleanup_setnull(texpackr_sheet** sheet)
{
	texpackr_sheet_free(*sheet);
	*sheet = NULL;
}

void test_common()
{
	bool result = false;
	texpackr_sheet* sheet = texpackr_sheet_new(SHEET_MAX_WIDTH, SHEET_MAX_HEIGHT);

	// 1. test individual insert
	printf("Test individual insertion\n");
	result = texpackr_sheet_insert_img(sheet, "assets/1.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image1.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	result = texpackr_sheet_insert_img(sheet, "assets/2.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image2.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	result = texpackr_sheet_insert_img(sheet, "assets/3.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image3.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}
	
	result = texpackr_sheet_insert_img(sheet, "assets/4.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image4.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	result = texpackr_sheet_insert_img(sheet, "assets/5.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image5.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	result = texpackr_sheet_insert_img(sheet, "assets/6.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image6.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	result = texpackr_sheet_insert_img(sheet, "assets/7.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image7.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	result = texpackr_sheet_insert_img(sheet, "assets/8.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image8.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}

	print_all_sprites(sheet);
	texpackr_sheet_export(sheet, "assets/sheet.png", "assets/sheet.tpr");

	// clear then we gonna test batch insert
	texpackr_sheet_clear(sheet);

	// form the array of string for using in batch insertion
#define BATCH_IMG_SIZE 8
	char* batch_img_filenames[BATCH_IMG_SIZE] = {
		"assets/1.png",
		"assets/2.png",
		"assets/3.png",
		"assets/4.png",
		"assets/5.png",
		"assets/6.png",
		"assets/7.png",
		"assets/8.png"
	};
	// 2. batch insert
	printf("Test batch insertion\n");
	result = texpackr_sheet_batch_imgs(sheet, (const char**)&batch_img_filenames, BATCH_IMG_SIZE);
	if (!result)
	{
		fprintf(stderr, "Error batch inserting assets\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}
	print_all_sprites(sheet);
	texpackr_sheet_export(sheet, "assets/sheet-batch.png", "assets/sheet-batch.tpr");

	// clear then we gonna test batch insert
	texpackr_sheet_clear(sheet);
	
	// 3. test inserting GRAY (1 channel) png image
	printf("Test inserting GRAY format (1 channel) png image\n");
	result = texpackr_sheet_insert_img(sheet, "assets/gray.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting gray.png\n");
    cleanup_setnull(&sheet);
    assert(result == true);
	}
	print_all_sprites(sheet);
	texpackr_sheet_export(sheet, "assets/sheet-gray.png", "assets/sheet-gray.tpr");

  // clean up, no need anymore
  cleanup_setnull(&sheet);

	// 4. parse meta
	texpackr_sheetmeta* meta = texpackr_parse("assets/sheet-batch.tpr");
  assert(meta != NULL);

	if (meta != NULL)
  {
		print_sheetmeta(meta);
		texpackr_sheetmeta_free(meta);
		meta = NULL;
  }

  printf("Checked test.c\n");
}
