#include "texpackr/sprite.h"
#include "texpackr/png_util.h"
#include <stdlib.h>

void texpackr_sprite_free(texpackr_sprite* sp)
{
	// free internals
	texpackr_sprite_free_internals(sp);

	// free itself
	free(sp);
}

void texpackr_sprite_free_internals(texpackr_sprite* sp)
{
	// free string
	free(sp->filename);
	sp->filename = NULL;

	// free image data (if any)
	if (sp->image_data != NULL)
	{
		texpackr_free_png_image_data((png_bytepp)sp->image_data, sp->size.y);
		sp->image_data = NULL;
	}

	// reset other attributes
	sp->offset.x = -1;
	sp->offset.y = -1;
	sp->size.x = -1;
	sp->size.y = -1;
}
