//
// Created by לידור on 15/06/2023.
//

#ifndef FINAL_PROJECT_SYMBOL_TABLE_H
#define FINAL_PROJECT_SYMBOL_TABLE_H

//@TODO change to symbol
typedef struct symbol_table{
    symbol **macros;
    unsigned int free_index;
} symbol_table;

macro_table *new_macro_table(void);

macro *new_macro(char *macro_name, char *macro_content);

void add_macro_to_table(macro_table *m_table, macro *mcr);

char *get_macro_content_from_table(macro_table *m_table, char *macro_name);

void update_macro_content(macro_table *m_table, char *macro_name, char *macro_update);

void free_macro_table(macro_table *m_table);

void print_macro_table(macro_table *m_table);

#endif //FINAL_PROJECT_SYMBOL_TABLE_H
