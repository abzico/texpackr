#ifndef TEXPACKR_META_H_
#define TEXPACKR_META_H_

#include "types.h"

/*
 * Parse sheet's companion meta file and returned meta info included sprite elements as hashmap for fast-retrieval.
 *
 * \param sheet_meta_filename sheet's meta filename
 * \return newly created sheet's meta info. Users need to free it with texpackr_sheetmeta_free() when done using it.
 */
extern texpackr_sheetmeta* texpackr_parse(const char* sheet_meta_filename);

/*
 * Free sheet's meta structure.
 */
extern void texpackr_sheetmeta_free(texpackr_sheetmeta* meta);

#endif
