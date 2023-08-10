/*
 Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "global_att.h"
#include "symbol_table.h"

#define SKIP_WHITE_CHAR(str, ind) \
        for(; str[(ind)] && (isspace(str[(ind)])); (++(ind))) \
        ;


char *str_cat(char *str1, char *str2);

bool is_empty(char c);

int string_num(char *str, char *delim);

char **strings(char *string, char *delim);

void free_string_p(char **strings, int string_num);

int skip_white_char(char *string, int index);
bool is_label(char *str);
char num_to_base_64(int val);
void print_error(line_content *line_c, char *format, ...);
void update_data_sym_address(symbol_table *s_table, unsigned int ic);
bool all_digit(char *str);