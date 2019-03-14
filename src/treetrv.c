#include <stdlib.h>
#include <png.h>
#include "texpackr/internal.h"
#include "texpackr/log.h"
#include "texpackr/png_util.h"

void texpackr_free_all_nodes(texpackr_node* n)
{
	if (n != NULL)
	{
		texpackr_free_all_nodes(n->child[0]);
		texpackr_free_all_nodes(n->child[1]);

		// delete child nodes
		n->child[0] = NULL;
		n->child[1] = NULL;

		TEXPACKR_LOG("free n with rect: %d, %d, %d, %d\n", n->rc.left, n->rc.top, n->rc.right, n->rc.bottom)

		free(n);
		n = NULL;
	}
}
