// test program for capability of texpackr
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "texpackr/texpackr.h"

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
			printf("sprite[%d] filename: %s, offset: %d,%d, size: %d,%d\n", i, sprite_ptr->filename, sprite_ptr->offset.x, sprite_ptr->offset.y, sprite_ptr->size.x, sprite_ptr->size.y);
		}
	}
}

static void print_sprite(const texpackr_sprite* sp)
{
	printf("sprite filename: %s, offset: %d,%d, size: %d,%d\n", sp->filename, sp->offset.x, sp->offset.y, sp->size.x, sp->size.y);
}

static void print_sheetmeta(texpackr_sheetmeta* meta)
{
	printf("meta info: size: %d,%d , sprite count in hashmap: %d\n", meta->size.x, meta->size.y, meta->sprites->size);

	// (known key)
	// printing known keys out, most likely we will known the key before hand so...
	const texpackr_sprite* sp = NULL;
	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/1.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/2.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/3.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/4.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/5.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/6.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/7.png");
	print_sprite(sp);

	sp = (const texpackr_sprite*)hashmapc_get(meta->sprites, "images/8.png");
	if (sp == NULL)
		printf("Not found sprite with key 'images/8.png'\n");
}

int main (int argc, char** argv)
{
	bool result = false;
	texpackr_sheet* sheet = texpackr_sheet_new(SHEET_MAX_WIDTH, SHEET_MAX_HEIGHT);

	// 1. test individual insert
	printf("Test individual insertion\n");
	result = texpackr_sheet_insert_img(sheet, "images/1.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image1.png\n");
		goto CLEANUP;
	}

	result = texpackr_sheet_insert_img(sheet, "images/2.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image2.png\n");
		goto CLEANUP;
	}

	result = texpackr_sheet_insert_img(sheet, "images/3.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image3.png\n");
		goto CLEANUP;
	}
	
	result = texpackr_sheet_insert_img(sheet, "images/4.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image4.png\n");
		goto CLEANUP;
	}

	result = texpackr_sheet_insert_img(sheet, "images/5.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image5.png\n");
		goto CLEANUP;
	}

	result = texpackr_sheet_insert_img(sheet, "images/6.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image6.png\n");
		goto CLEANUP;
	}

	result = texpackr_sheet_insert_img(sheet, "images/7.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image7.png\n");
		goto CLEANUP;
	}

	result = texpackr_sheet_insert_img(sheet, "images/8.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting image8.png\n");
		goto CLEANUP;
	}

	print_all_sprites(sheet);
	texpackr_sheet_export(sheet, "images/sheet.png", "images/sheet.tpr");

	// clear then we gonna test batch insert
	texpackr_sheet_clear(sheet);

	// form the array of string for using in batch insertion
#define BATCH_IMG_SIZE 8
	char* batch_img_filenames[BATCH_IMG_SIZE] = {
		"images/1.png",
		"images/2.png",
		"images/3.png",
		"images/4.png",
		"images/5.png",
		"images/6.png",
		"images/7.png",
		"images/8.png"
	};
	// 2. batch insert
	printf("Test batch insertion\n");
	result = texpackr_sheet_batch_imgs(sheet, (const char**)&batch_img_filenames, BATCH_IMG_SIZE);
	if (!result)
	{
		fprintf(stderr, "Error batch inserting images\n");
		goto CLEANUP;
	}
	print_all_sprites(sheet);
	texpackr_sheet_export(sheet, "images/sheet-batch.png", "images/sheet-batch.tpr");

	// clear then we gonna test batch insert
	texpackr_sheet_clear(sheet);
	
	// 3. test inserting GRAY (1 channel) png image
	printf("Test inserting GRAY format (1 channel) png image\n");
	result = texpackr_sheet_insert_img(sheet, "images/gray.png");
	if (!result)
	{
		fprintf(stderr, "Error inserting gray.png\n");
		goto CLEANUP;
	}
	print_all_sprites(sheet);
	texpackr_sheet_export(sheet, "images/sheet-gray.png", "images/sheet-gray.tpr");

	// 4. parse meta
	texpackr_sheetmeta* meta = texpackr_parse("images/sheet-batch.tpr");
	if (meta != NULL)
		print_sheetmeta(meta);

	// clear meta
	if (meta != NULL)
	{
		texpackr_sheetmeta_free(meta);
		meta = NULL;
	}

CLEANUP:

	// done, then clear sheet
	texpackr_sheet_free(sheet);
	sheet = NULL;
  return 0;
}
