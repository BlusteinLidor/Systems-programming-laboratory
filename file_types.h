//
// Created by לידור on 21/07/2023.
//

#ifndef FINAL_PROJECT_FILE_TYPES_H
#define FINAL_PROJECT_FILE_TYPES_H
#include "global_att.h"
#include <stdio.h>
#include "other_functions.h"
#include "symbol_table.h"

void file_type_ob(char *file_name, code_m_word code_m[], data_m_word data_m[],
                  unsigned int ic, unsigned int dc);

void file_type_ext(char *file_name, symbol_table *s_table, code_m_word code_m[],
                   data_m_word data_m[], unsigned int ic);

void file_type_ent(char *file_name, symbol_table *s_table);

#endif //FINAL_PROJECT_FILE_TYPES_H
