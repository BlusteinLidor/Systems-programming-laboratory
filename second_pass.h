

#ifndef FINAL_PROJECT_SECOND_PASS_H
#define FINAL_PROJECT_SECOND_PASS_H

#include "global_att.h"
#include "ast.h"
#include "other_functions.h"
#include "symbol_table.h"

bool second_pass_process_line(line_content line_c, symbol_table *s_table, ast as_tree);
void second_pass_process_label(symbol_table *s_table, code_m_word *code_m, unsigned int ic);

#endif
