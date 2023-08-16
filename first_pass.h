/* This file underlines the first pass process - it goes
 * through each line of the file and makes the machine code
 * according to each word*/

#ifndef FINAL_PROJECT_FIRST_PASS_H
#define FINAL_PROJECT_FIRST_PASS_H

/* this function goes through a line and processes the
 * line's machine code - if the line's has label declaration,
 * whether it's an instruction or directive
 * @param ic - instruction counter
 * @param dc - data counter
 * @param line_c - line content
 * @param s_table - symbol table
 * @param d_word - data word machine code
 * @param c_word - machine word
 * @param as_tree - Abstract syntax tree
 * @return true - if there were no errors */
bool first_pass_process_line(unsigned int *ic, unsigned int *dc, line_content line_c,
                             symbol_table *s_table, data_m_word *d_word,
                             code_m_word *c_word, ast *as_tree);

#endif
