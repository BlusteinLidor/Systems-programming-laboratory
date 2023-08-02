

#ifndef FINAL_PROJECT_INST_PROCESS_H
#define FINAL_PROJECT_INST_PROCESS_H

#include <stdlib.h>
#include "other_functions.h"
#include "ast.h"
#include "global_att.h"

bool process_inst(line_content *line_c, unsigned int *ic, symbol_table *s_table, code_m_word *code_m, ast *as_tree);

#endif
