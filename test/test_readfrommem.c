// test reading .ptr file from mem
#include <stdio.h>
#include <string.h>
#include "test_common.h"

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

int main(void)
{
  FILE* file = fopen("assets/sheet-batch.tpr", "rb");
  if (file == NULL)
  {
    fprintf(stderr, "Cannot open %s file\n", "assets/sheet-batch.tpr");
    return 1;
  }

  // find file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  unsigned char file_buffer[file_size];
  memset(file_buffer, 0, file_size);
  if (fread(file_buffer, file_size, 1, file) != 1)
  { // close file
    fclose(file);
    file = NULL;

    fprintf(stderr, "Error reading file\n");
    return 1;
  }
  // close file
  fclose(file);
  file = NULL;

	// parse meta file from memory
	texpackr_sheetmeta* meta = texpackr_parse_mem(file_buffer, file_size);
	if (meta != NULL)
		print_sheetmeta(meta);
  else
  {
    // error
    fclose(file);
    file = NULL;
    fprintf(stderr, "texpackr_parse_mem function should not return NULL");
    return 1;
  }

	// clear meta
  texpackr_sheetmeta_free(meta);
  meta = NULL;

  // in turn log it into .log
  printf("Checked test-readfromem.c\n");

  return 0;
}
