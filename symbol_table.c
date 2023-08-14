#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include "global_att.h"

symbol_table *new_symbol_table(void){
    int i = 0;
    /* allocating memory for the s_table */
    symbol_table *s_table = malloc(sizeof(symbol_table));
    /* allocating memory for the individual symbols */
    s_table->symbols = malloc(sizeof(symbol) * TABLE_SIZE);
    /* initializing the individual symbols to NULL */
    for(; i < TABLE_SIZE; i++){
        s_table->symbols[i] = NULL;
    }
    /* first free index initialized to 0 */
    s_table->free_index = 0;

    return s_table;
}

symbol *new_symbol(char *symbol_name, unsigned int symbol_address, symbol_type symbol_t){
    /* allocate memory for the symbol */
    symbol *new_s = malloc(sizeof(symbol));
    /* allocate memory for the symbol name */
    new_s->symbol_name = malloc(strlen(symbol_name));
    new_s->symbol_address = symbol_address;
    strcpy(new_s->symbol_name, symbol_name);
    new_s->symbol_t = symbol_t;

    return new_s;
}

void add_symbol_to_table(symbol_table *s_table, symbol *smbl){
    /* add the symbol in the free index spot */
    s_table->symbols[s_table->free_index] = smbl;
    /* increment free index */
    s_table->free_index++;
}

symbol_type get_symbol_type_from_table(symbol_table *s_table, char *symbol_name){
    int i = 0;
    /* loop over all available symbols in the table and compare names*/
    for(; i < s_table->free_index; i++){
        if(strcmp(s_table->symbols[i]->symbol_name, symbol_name) == 0) {
            /* return symbol type */
            return s_table->symbols[i]->symbol_t;
        }
    }
    /* if symbol wasn't found - return error type */
    return error_symbol;
}

symbol *get_symbol_from_table(symbol_table *s_table, char *symbol_name){
    int i = 0;
    /* loop over all available symbols in the table and compare names*/
    for(; i < s_table->free_index; i++){
        if(strcmp(s_table->symbols[i]->symbol_name, symbol_name) == 0){
            /* return symbol */
            return s_table->symbols[i];
        }
    }
    /* if symbol wasn't found - return null */
    return NULL;
}

void free_symbol_table(symbol_table *s_table){
    int i = 0;
    symbol *curr_symbol;
    /* loop over all available symbols in the table and free their memory */
    for(; i < s_table->free_index; i++){
        if((curr_symbol = s_table->symbols[i]) != NULL){
            free(curr_symbol->symbol_name);
            free(curr_symbol);
        }
    }
    /* after freeing all symbols - set free_index to 0 */
    s_table->free_index = 0;
    /* free the table */
    free(s_table->symbols);
    free(s_table);
}
