/* This file underlines the symbol and symbol table */

#ifndef FINAL_PROJECT_SYMBOL_TABLE_H
#define FINAL_PROJECT_SYMBOL_TABLE_H

#include "global_att.h"

/* a symbol - holding:
 * symbol name
 * symbol address (ic)
 * symbol type (instruction/data/extern/entry) */
typedef struct symbol{
    char *symbol_name;
    unsigned int symbol_address;
    symbol_type symbol_t;
} symbol;

/* symbol table - containing all symbols
 * free index - pointing to the next free space in the
 * symbol table */
typedef struct symbol_table{
    symbol **symbols;
    unsigned int free_index;
} symbol_table;

/* creates a new symbol table
 * @return pointer to a new symbol table */
symbol_table *new_symbol_table(void);

/* creates a new symbol
 * @param symbol_name - string - the symbol name
 * @param symbol_address - the symbol address (ic)
 * @param symbol_t - the symbol type
 * @return pointer to a symbol */
symbol *new_symbol(char *symbol_name, unsigned int symbol_address, symbol_type symbol_t);

/* adds a symbol to a table
 * @param s_table - the symbol table
 * @param smbl - the symbol */
void add_symbol_to_table(symbol_table *s_table, symbol *smbl);

/* gets a symbol's type from a table by its name
 * @param s_table - the symbol table
 * @param symbol_name - string - the symbol name
 * @return symbol_type */
symbol_type get_symbol_type_from_table(symbol_table *s_table, char *symbol_name);

/* free the symbol table memory
 * @param s_table - the symbol table */
void free_symbol_table(symbol_table *s_table);

/* gets a symbol from a table by its name
 * @param s_table - the symbol table
 * @param symbol_name - string - the symbol name
 * @return symbol */
symbol *get_symbol_from_table(symbol_table *s_table, char *symbol_name);

#endif
