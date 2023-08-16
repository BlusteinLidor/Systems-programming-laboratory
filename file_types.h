/* This file makes the next type files: .ob, .ext, .ent */

#ifndef FINAL_PROJECT_FILE_TYPES_H
#define FINAL_PROJECT_FILE_TYPES_H
#include "global_att.h"
#include <stdio.h>
#include "other_functions.h"
#include "symbol_table.h"

/* make the file type .ob
 * @param file_name - file name without the format
 * @code_m[] - machine word array
 * data_m[] - data word array
 * ic - instruction counter
 * dc - data counter */
void file_type_ob(char *file_name, code_m_word code_m[], data_m_word data_m[],
                  unsigned int ic, unsigned int dc);

/* make the file type .ext
 * @param file_name - file name without the format
 * @param s_table - symbol table
 * @param code_m[] - machine word array
 * @param data_m[] - data word array
 * @param ic - instruction counter */
void file_type_ext(char *file_name, symbol_table *s_table, code_m_word code_m[],
                   data_m_word data_m[], unsigned int ic);

/* make the file type .ent
 * @param file_name - file name without the format
 * @param s_table - symbol table */
void file_type_ent(char *file_name, symbol_table *s_table);

#endif
