/* This file underlines the macro and macro table */

#ifndef FINAL_PROJECT_MACRO_TABLE_H
#define FINAL_PROJECT_MACRO_TABLE_H

/* a macro - holding:
 * macro name
 * macro content */
typedef struct macro{
    char *macro_name;
    char *macro_content;
} macro;

/* macro table - containing all macros
 * free index - pointing to the next free space in the
 * macro table */
typedef struct macro_table{
    macro **macros;
    unsigned int free_index;
} macro_table;

/* creates a new macro table
 * @return pointer to a new macro table */
macro_table *new_macro_table(void);

/* creates a new macro
 * @param macro_name - string - the macro name
 * @param macro_content - the definition of the macro
 * @return pointer to a macro */
macro *new_macro(char *macro_name, char *macro_content);

/* adds a macro to a table
 * @param m_table - the macro table
 * @param mcr - the macro */
void add_macro_to_table(macro_table *m_table, macro *mcr);

/* gets macro content from a table by its name
 * @param m_table - the macro table
 * @param macro_name - string - the macro name
 * @return macro content */
char *get_macro_content_from_table(macro_table *m_table, char *macro_name);

/* updates macro content in a table by its name
 * @param m_table - the macro table
 * @param macro_name - string - the macro name
 * @param macro_update - string - the updated macro content */
void update_macro_content(macro_table *m_table, char *macro_name, char *macro_update);

/* free the macro table memory
 * @param m_table - the macro table */
void free_macro_table(macro_table *m_table);



#endif

