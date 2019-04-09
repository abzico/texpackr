#ifndef TEXPACKR_SPRITE_H_
#define TEXPACKR_SPRITE_H_

#include "texpackr/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Free individual sprite structure.
 *
 * \param sp sprite
 */
extern void texpackr_sprite_free(texpackr_sprite* sp);

/*
 * Free individual sprite's internals.
 *
 * \param sp sprite.
 */
extern void texpackr_sprite_free_internals(texpackr_sprite* sp);

#ifdef __cplusplus
}
#endif

#endif
