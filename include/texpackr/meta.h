#ifndef TEXPACKR_META_H_
#define TEXPACKR_META_H_

#include "texpackr/types.h"

/*
 * Parse sheet's companion meta file and returned meta info included sprite elements as hashmap for fast-retrieval.
 *
 * \param sheet_meta_filename sheet's meta filename
 * \return newly created sheet's meta info. Users need to free it with texpackr_sheetmeta_free() when done using it. Otherwise return NULL if error occurs.
 */
extern texpackr_sheetmeta* texpackr_parse(const char* sheet_meta_filename);

/*
 * Create a new sheet's meta structure.
 *
 * \return newly created sheet's meta structure. Users has responsibility to free it with texpackr_sheetmeta_free().
 */
extern texpackr_sheetmeta* texpackr_sheetmeta_new();

/*
 * Free sheet's meta structure.
 */
extern void texpackr_sheetmeta_free(texpackr_sheetmeta* meta);

#endif