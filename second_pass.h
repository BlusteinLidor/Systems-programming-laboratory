/* This file underlines the second pass in the assembler -
 * It goes through the file once again, and completes the
 * machine code */

#ifndef FINAL_PROJECT_SECOND_PASS_H
#define FINAL_PROJECT_SECOND_PASS_H

#include "global_att.h"
#include "ast.h"
#include "other_functions.h"
#include "symbol_table.h"

/* goes through a line and checks if an extern/entry label
 * is defined there
 * @param line_c - the line info (file name, content and line number)
 * @param s_table - symbol table
 * @param as_tree - Abstract syntax tree that stores a line
 * @return true if there was no error */
bool second_pass_process_line(line_content line_c, symbol_table *s_table, ast as_tree);

/* processes a label into machine code
 * @param s_table - symbol table
 * @param code_m - the machine code image
 * @param ic - instruction counter */
void second_pass_process_label(symbol_table *s_table, code_m_word *code_m, unsigned int ic);

#endif
