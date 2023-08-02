

#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include "global_att.h"
#include <stdio.h>
#include <stdlib.h>

symbol_table *new_symbol_table(void){
    int i = 0;
    /* allocating memory for the s_table */
    symbol_table *s_table = malloc(sizeof(symbol_table));
    /* allocating memory for the individual macros */
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
    symbol *new_s = malloc(sizeof(symbol));
    new_s->symbol_name = malloc(strlen(symbol_name));
    new_s->symbol_address = symbol_address;
    strcpy(new_s->symbol_name, symbol_name);

    return new_s;
}

void add_symbol_to_table(symbol_table *s_table, symbol *smbl){
    s_table->symbols[s_table->free_index] = smbl;
    s_table->free_index++;
}

unsigned int get_symbol_address_from_table(symbol_table *s_table, char *symbol_name){
    int i = 0;
    for(; i < s_table->free_index; i++){
        if(strcmp(s_table->symbols[i]->symbol_name, symbol_name) == 0) {
            return s_table->symbols[i]->symbol_address;
        }
    }
    return -1;
}

symbol_type get_symbol_type_from_table(symbol_table *s_table, char *symbol_name){
    int i = 0;
    for(; i < s_table->free_index; i++){
        if(strcmp(s_table->symbols[i]->symbol_name, symbol_name) == 0) {
            return s_table->symbols[i]->symbol_t;
        }
    }
    return error_symbol;
}

symbol *get_symbol_from_table(symbol_table *s_table, char *symbol_name){
    int i = 0;
    for(; i < s_table->free_index; i++){
        if(strcmp(s_table->symbols[i]->symbol_name, symbol_name) == 0){
            return s_table->symbols[i];
        }
    }
    return NULL;
}

void free_symbol_table(symbol_table *s_table){
    int i = 0;
    symbol *curr_symbol;
    for(; i < s_table->free_index; i++){
        if((curr_symbol = s_table->symbols[i]) != NULL){
            free(curr_symbol->symbol_name);
            free(curr_symbol);
        }
    }
    s_table->free_index = 0;
    free(s_table->symbols);
    free(s_table);
}

void print_symbol_table(symbol_table *s_table){
    int i = 0;
    symbol *curr_symbol;
    for(; i < s_table->free_index; i++){
        if((curr_symbol = s_table->symbols[i]) != NULL){
            printf("symbol #%d of type %d = %s: %d\n", (i+1), curr_symbol->symbol_t, curr_symbol->symbol_name, ((curr_symbol->symbol_address) + 100));
        }
    }
}