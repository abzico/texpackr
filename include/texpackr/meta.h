#ifndef TEXPACKR_META_H_
#define TEXPACKR_META_H_

#include <stdbool.h>
#include "texpackr/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Parse sheet's companion meta file and returned meta info included sprite elements as hashmap for fast-retrieval.
 *
 * \param sheet_meta_filename sheet's meta filename
 * \return newly created sheet's meta info. Users need to free it with texpackr_sheetmeta_free() when done using it. Otherwise return NULL if error occurs.
 */
extern texpackr_sheetmeta* texpackr_parse(const char* sheet_meta_filename);

/*
 * Parse sheet's companion meta file and returned meta info included sprite elements as hashmap for fast-retrieval.
 *
 * Use this function in case you have read the file via other ways, and got hold of file's data buffer as pointer. Thus this function doesn't care which method you used in reading a file. It can be useful for portability code i.e. reading file from Android.
 *
 * \param file_base sheet meta file starting file location in memory.
 * \param file_size sheet meta file's size in bytes
 * \return newly created sheet's meta info. Users need to free it with texpackr_sheetmeta_free() when done using it. Otherwise return NULL if error occurs.
 */
extern texpackr_sheetmeta* texpackr_parse_mem(const unsigned char* file_base, long file_size);

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

/*
 * Write meta file.
 *
 * \param sheet_meta_filename file name to write meta file
 * \param s sheet used as info to write into meta file
 * \return true if successfully write file, otherwise false returned.
 */
extern bool texpackr_sheetmeta_write_meta_file(const char* sheet_meta_filename, const texpackr_sheet* s);

#ifdef __cplusplus
}
#endif

#endif
