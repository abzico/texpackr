#include "texpackr_meta.h"
#include <stdio.h>
#include <stdlib.h>

texpackr_sheetmeta* texpackr_parse(const char* sheet_meta_filename)
{
	return NULL;
}

void texpackr_sheetmeta_free(texpackr_sheetmeta* meta)
{
	// free internal hashmap
	hashmapc_free(meta->sprites);
	meta->sprites = NULL;

	// free itself
	free(meta);
}
