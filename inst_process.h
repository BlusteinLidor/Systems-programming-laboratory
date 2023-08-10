

#ifndef FINAL_PROJECT_INST_PROCESS_H
#define FINAL_PROJECT_INST_PROCESS_H

#include <stdlib.h>
#include "other_functions.h"
#include "ast.h"
#include "global_att.h"

bool process_inst(line_content *line_c, unsigned int *ic, code_m_word *code_m, ast *as_tree);
void process_first_word(unsigned int *ic, code_m_word code_m[], op_code op,
                        operand_type_num first_op, operand_type_num second_op);

#endif
