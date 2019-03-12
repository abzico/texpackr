#include <stdio.h>
#include <stdbool.h>
#include "texpackr.h"

#define SHEET_MAX_WIDTH 2048
#define SHEET_MAX_HEIGHT 2048

void print_all_sprites(texpackr_sheet* s)
{
	for (int i=0; i<s->sprite_count; i++)
	{
		texpackr_sprite* sprite_ptr = s->sprites + i;
		printf("sprite[%d] filename: %s, offset: %d,%d, size: %d,%d\n", i, sprite_ptr->filename, sprite_ptr->offset.x, sprite_ptr->offset.y, sprite_ptr->size.x, sprite_ptr->size.y);
	}
}

int main (int argc, char** argv)
{
	bool result = false;
	texpackr_sheet* sheet = texpackr_sheet_new(SHEET_MAX_WIDTH, SHEET_MAX_HEIGHT);

	result = texpackr_sheet_insert_img(sheet, "images/1.png");
	if (!result)
		fprintf(stderr, "Error inserting image 1.png\n");

	result = texpackr_sheet_insert_img(sheet, "images/2.png");
	if (!result)
		fprintf(stderr, "Error inserting image 2.png\n");

	result = texpackr_sheet_insert_img(sheet, "images/3.png");
	if (!result)
		fprintf(stderr, "Error inserting image 3.png\n");
	
	result = texpackr_sheet_insert_img(sheet, "images/4.png");
	if (!result)
		fprintf(stderr, "Error inserting image 4.png\n");

	result = texpackr_sheet_insert_img(sheet, "images/5.png");
	if (!result)
		fprintf(stderr, "Error inserting image 5.png\n");

	result = texpackr_sheet_insert_img(sheet, "images/6.png");
	if (!result)
		fprintf(stderr, "Error inserting image 6.png\n");

	result = texpackr_sheet_insert_img(sheet, "images/7.png");
	if (!result)
		fprintf(stderr, "Error inserting image 7.png\n");

	print_all_sprites(sheet);

	texpackr_sheet_free(sheet);
	sheet = NULL;
  return 0;
}
