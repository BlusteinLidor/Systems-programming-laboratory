/* This file underlines the processing of instruction lines
 * into machine code */

#ifndef FINAL_PROJECT_INST_PROCESS_H
#define FINAL_PROJECT_INST_PROCESS_H

#include <stdlib.h>
#include "other_functions.h"
#include "ast.h"
#include "global_att.h"

/* process the instruction line into machine code
 * @param line_c - line information
 * @param ic - instruction count
 * @param s_table - symbol table
 * @param code_m - the machine word
 * @param as_tree - the Abstract syntax tree
 * @return true - if there were no errors */
bool process_inst(line_content *line_c, unsigned int *ic, symbol_table *s_table, code_m_word *code_m, ast *as_tree);

/* process the first word in an instruction line
 * @param ic - instruction count
 * @param code_m[] - machine word array
 * @param op - op code info
 * @param first_op - first operand type number
 * @param second_op - second operand type number */
void process_first_word(unsigned int *ic, code_m_word code_m[], op_code op,
                        operand_type_num first_op, operand_type_num second_op);

#endif
