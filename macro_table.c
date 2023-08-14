#include <stdlib.h>
#include <string.h>
#include "macro_table.h"
#include "global_att.h"

macro_table *new_macro_table(void){
    int i = 0;
    /* allocating memory for the m_table */
    macro_table *m_table = malloc(sizeof(macro_table));
    /* allocating memory for the individual macros */
    m_table->macros = malloc(sizeof(macro) * TABLE_SIZE);
    /* initializing the individual macros to NULL */
    for(; i < TABLE_SIZE; i++){
        m_table->macros[i] = NULL;
    }
    /* first free index initialized to 0 */
    m_table->free_index = 0;

    return m_table;
}

macro *new_macro(char *macro_name, char *macro_content){
    /* allocate memory for the macro */
    macro *new_m = malloc(sizeof(macro));
    /* allocate memory for the macro name */
    new_m->macro_name = malloc(MAX_MACRO_NAME_SIZE);
    /* allocate memory for the macro content */
    new_m->macro_content = malloc(MAX_LINE_LENGTH);
    strcpy(new_m->macro_name, macro_name);
    strcpy(new_m->macro_content, macro_content);

    return new_m;
}

void add_macro_to_table(macro_table *m_table, macro *mcr){
    /* add the macro in the free index spot */
    m_table->macros[m_table->free_index] = mcr;
    /* increment free index */
    m_table->free_index++;
}

char *get_macro_content_from_table(macro_table *m_table, char *macro_name){
    int i = 0;
    /* loop over all available macros in the table and compare names*/
    for(; i < m_table->free_index; i++){
        if(strcmp(m_table->macros[i]->macro_name, macro_name) == 0){
            /* return macro content */
            return m_table->macros[i]->macro_content;
        }
    }
    /* if macro wasn't found - return null */
    return NULL;
}

void update_macro_content(macro_table *m_table, char *macro_name, char *macro_update){
    int i = 0;
    /* loop over all available macros in the table and compare names*/
    for(; i < m_table->free_index; i++){
        if(strcmp(m_table->macros[i]->macro_name, macro_name) == 0){
            /* update macro content */
            m_table->macros[i]->macro_content = macro_update;
        }
    }
}

void free_macro_table(macro_table *m_table){
    int i = 0;
    macro *curr_macro;
    /* loop over all available macros in the table and free their memory */
    for(; i < m_table->free_index; i++){
        if((curr_macro = m_table->macros[i]) != NULL){
            free(curr_macro->macro_name);
            free(curr_macro->macro_content);
            free(curr_macro);
        }
    }
    /* after freeing all macros - set free_index to 0 */
    m_table->free_index = 0;
    /* free the table */
    free(m_table->macros);
    free(m_table);
}






