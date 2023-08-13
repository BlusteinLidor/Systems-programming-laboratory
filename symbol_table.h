

#ifndef FINAL_PROJECT_SYMBOL_TABLE_H
#define FINAL_PROJECT_SYMBOL_TABLE_H

#include "global_att.h"

/* @TODO check if needed to add delete_symbol function */

typedef struct symbol{
    char *symbol_name;
    unsigned int symbol_address;
    symbol_type symbol_t;
} symbol;

typedef struct symbol_table{
    symbol **symbols;
    unsigned int free_index;
} symbol_table;



symbol_table *new_symbol_table(void);

symbol *new_symbol(char *symbol_name, unsigned int symbol_address, symbol_type symbol_t);

void add_symbol_to_table(symbol_table *s_table, symbol *smbl);

int get_symbol_address_from_table(symbol_table *s_table, char *symbol_name);

symbol_type get_symbol_type_from_table(symbol_table *s_table, char *symbol_name);

void free_symbol_table(symbol_table *s_table);

void print_symbol_table(symbol_table *s_table);

symbol *get_symbol_from_table(symbol_table *s_table, char *symbol_name);

#endif
