/*
 Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <string.h>
#include "macro_table.h"
#include "globals.h"
#include <stdio.h>

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
    macro *new_m = malloc(sizeof(macro));
    new_m->macro_name = malloc(strlen(macro_name));
    new_m->macro_content = malloc(strlen(macro_content));
    strcpy(new_m->macro_name, macro_name);
    strcpy(new_m->macro_content, macro_content);

    return new_m;
}

void add_macro_to_table(macro_table *m_table, macro *mcr){
    m_table->macros[m_table->free_index] = mcr;
    m_table->free_index++;
}

char *get_macro_content_from_table(macro_table *m_table, char *macro_name){
    int i = 0;
    for(; i < m_table->free_index; i++){
        if(strcmp(m_table->macros[i]->macro_name, macro_name) == 0){
            return m_table->macros[i]->macro_content;
        }
    }
    return NULL;
}

void update_macro_content(macro_table *m_table, char *macro_name, char *macro_update){
    int i = 0;
    for(; i < m_table->free_index; i++){
        if(strcmp(m_table->macros[i]->macro_name, macro_name) == 0){
            m_table->macros[i]->macro_content = macro_update;
        }
    }
}

void free_macro_table(macro_table *m_table){
    int i = 0;
    macro *curr_macro;
    for(; i < m_table->free_index; i++){
        if((curr_macro = m_table->macros[i]) != NULL){
            free(curr_macro->macro_name);
            free(curr_macro->macro_content);
            free(curr_macro);
        }
    }
    m_table->free_index = 0;
    free(m_table->macros);
    free(m_table);
}

void print_macro_table(macro_table *m_table){
    int i = 0;
    macro *curr_macro;
    for(; i < m_table->free_index; i++){
        if((curr_macro = m_table->macros[i]) != NULL){
            printf("macro #%d = %s:\n%s\n", (i+1), curr_macro->macro_name, curr_macro->macro_content);
        }
    }
}





