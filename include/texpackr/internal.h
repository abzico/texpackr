#ifndef TEXPACKR_INTERNAL_H_
#define TEXPACKR_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * rectangle to represent area of each node
 */
typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
} texpackr_rect;

/*
 * node used in packing algorithm
 */
struct texpackr_node
{
	/// it can spawn new node either to left or right
	struct texpackr_node* child[2];

	/// rectangle representing area of a node
	texpackr_rect rc;

	/// use to refer to the image
	/// in which finally expose filename for user to access in the game code
	/// if it's -1 (which is by default) there's no image associated with this node yet
	int image_id;
};
typedef struct texpackr_node texpackr_node;

#ifdef __cplusplus
}
#endif

#endif
