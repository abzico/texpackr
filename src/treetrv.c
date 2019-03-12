#include <stdlib.h>
#include "texpackr_internal.h"

void texpackr_free_all_nodes(texpackr_node* n)
{
	if (n != NULL)
	{
		texpackr_free_all_nodes(n->child[0]);
		texpackr_free_all_nodes(n->child[1]);

		// delete the node
		n->child[0] = NULL;
		n->child[1] = NULL;
		free(n);
		n = NULL;
	}
}
