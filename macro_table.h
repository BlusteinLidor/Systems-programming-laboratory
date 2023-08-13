

#ifndef FINAL_PROJECT_MACRO_TABLE_H
#define FINAL_PROJECT_MACRO_TABLE_H

typedef struct macro{
    char *macro_name;
    char *macro_content;
} macro;

typedef struct macro_table{
    macro **macros;
    unsigned int free_index;
} macro_table;

macro_table *new_macro_table(void);

macro *new_macro(char *macro_name, char *macro_content);

void add_macro_to_table(macro_table *m_table, macro *mcr);

char *get_macro_content_from_table(macro_table *m_table, char *macro_name);

void update_macro_content(macro_table *m_table, char *macro_name, char *macro_update);

void free_macro_table(macro_table *m_table);

void print_macro_table(macro_table *m_table);

/* @TODO check if needed to add delete_macro function */



#endif

