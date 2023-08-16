/* This files holds all the extra functions needed for the
 * project, but have more general purposes, and less specific,
 * among them - string manipulations and checks, and more*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "global_att.h"
#include "symbol_table.h"

/* skip white characters macro */
#define SKIP_WHITE_CHAR(str, ind) \
        for(; str[(ind)] && (isspace(str[(ind)])); (++(ind))) \
        ;

/* 2 strings concatenations
 * @param str1 - first string
 * @param str2 - second string
 * @return concatenated string */
char *str_cat(char *str1, char *str2);

/* checks if the character is the end of line/file/comment line
 * @param c - character to check
 * @return true - if end of line/file or comment line */
bool is_empty(char c);

/* returns number of strings split
 * @param str - the line to split
 * @delim - the delimiters to split with
 * @return number of split strings */
int string_num(char *str, char *delim);

/* array of split strings
 * @param string - line to split
 * @delim - the delimiters to split with
 * @return string array */
char **strings(char *string, char *delim);

/* free memory of the string array
 * @param strings - string array
 * @param string_num - number of strings in the array
 */
void free_string_p(char **strings, int string_num);

/* skip white characters
 * @param string - the line
 * @param index - from which index to start
 * @return index after first white spaces */
int skip_white_char(char *string, int index);

/* checks if a string is can be a valid label
 * @param - str - the string to check
 * @return true - if the string is a valid label */
bool is_label(char *str);

/* converts machine code to base 64
 * @param val - the number to convert
 * @return the character to print (in base 64) */
char num_to_base_64(int val);

/* print error according to given arguments
 * @param line_c - the line information and content
 * @param format - the message to print
 * @param ... - arguments for the format */
void print_error(line_content *line_c, char *format, ...);

/* update the data symbol address table
 * @param s_table - symbol table
 * @param ic - the instruction counter */
void update_data_sym_address(symbol_table *s_table, unsigned int ic);

/* checks if the string contains digits only
 * @param str - the string to check
 * @return true if all characters are digits */
bool all_digit(char *str);