#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "global_att.h"
#include <stdarg.h>
#include "symbol_table.h"

#define RESERVED_WORDS 25

int skip_white_char(char *string, int index){
    /* as long as the string has not ended and there is space -
     * increment the index*/
    for(; string[index] && isspace(string[index]); (index)++);
    return index;
}

char *str_cat(char *str1, char *str2){
    /* allocate memory for the concatenated string */
    char *final_str = (char *) malloc(strlen(str1) + strlen(str2) + 1);
    /* add first string */
    strcpy(final_str, str1);
    /* add second string */
    strcat(final_str, str2);
    /* return full string */
    return final_str;
}

bool is_empty(char c){
    /* if the character is end of line/end of file or comment line -
     * return true */
    if(c == '\n' || c == '\0' || c == EOF || c == ';'){
        return true;
    }
    /* else - return false */
    return false;
}

int string_num(char *str, char *delim){
    int count = 0;
    char *temp;
    /* allocate memory for the string - to not change the original */
    char *copy = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
    strcpy(copy, str);

    /* use strtok to break a string to a few tokens using
     * delimiters */
    temp = strtok(copy, delim);
    /* as long as we didn't get to end of string*/
    while(temp != NULL){
        /* increment count, and move to the next break */
        count++;
        temp = strtok(NULL, delim);
    }
    /* free allocated memory and return count */
    free(copy);
    return count;
}

char **strings(char *string, char *delim){
    int str_num , i = 0;
    char *token;
    char **strings;
    /* allocate memory */
    char *copy = (char *) malloc(sizeof(char) * (MAX_LINE_LENGTH + 1));
    char *temp = (char *) malloc(sizeof(char) * (MAX_LINE_LENGTH + 1));
    strcpy(copy, string);
    /* get number of spaced strings */
    str_num = string_num(copy, delim);
    strings = malloc(sizeof(char *) * str_num);
    /* if couldn't allocate memory for the string array -
     * free all allocated memory and return null*/
    if(strings == NULL){
        free(strings);
        free(temp);
        free(copy);
        return NULL;
    }
    /* set token to the first token of strtok*/
    token = strtok(copy, delim);

    /* while the token is not null (end of string) or
     * the number of strings is not bigger than the number
     * of strings we checked*/
    while(token != NULL && i < str_num){
        strcpy(temp, token);
        strings[i] = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        /* if the memory allocation didn't succeed - free
         * memory and return null */
        if(strings[i] == NULL){
            free(strings[i]);
            free(strings);
            free(temp);
            free(copy);
            return NULL;
        }
        /* else - copy the token to the appropriate
         * spot in the string array */
        strcpy(strings[i++], temp);
        /* get to next token */
        token = strtok(NULL, delim);
    }
    /* free memory and return the array */
    free(temp);
    free(copy);
    return strings;
}

void free_string_p(char **strings, int string_num){
    int i;
    /* free all strings inside the array */
    for(i = 0; i < string_num; i++){
        free(strings[i]);
    }
    /* free array */
    free(strings);
}

bool is_label(char *str){
    char label[MAX_LABEL_SIZE + 1];
    int i;
    char *not_label_names[] = {"r0", "r1", "r2", "r3", "r4",
                               "r5", "r6", "r7", "and", "mov",
                               "cmp", "add", "sub", "not",
                               "clr", "lea","inc", "dec",
                               "jmp", "bne", "red", "prn",
                               "jsr", "rts", "stop"};
    /* number of reserved words */
    int nln_len = RESERVED_WORDS;
    /* label to check length */
    int str_len = strlen(str);
    /* if string length is more than the maximum label length
     * return false */
    if(str_len > MAX_LABEL_SIZE){
        return false;
    }
    /* if the last character in the label is ':' (as should
     * be in every label definition) */
    if(str[str_len - 1] == ':'){
        /* copy the string without the ':' */
        strncpy(label, str, str_len - 1);
        label[str_len] = '\0';
    }
    else{
        strncpy(label, str, str_len);
        label[str_len] = '\0';
    }
    /* if the first character in the string is not an
     * alphabetic character - return false (not valid) */
    if(!isalpha(label[0])){
        return false;
    }
    /* check if the string is not one of the reserved
     * words - if it is - return false */
    for(i = 0; i < nln_len; i++){
        if(strcmp(label, not_label_names[i]) == 0){
            return false;
        }
    }
    /* check if the string doesn't contain any non-alphanumeric
     * characters. If it does - return false */
    for(i = 0; label[i] != '\0'; i++){
        if(!isalnum(label[i])){
            return false;
        }
    }

    return true;
}

bool all_digit(char *str){
    /* go thought all characters of a string and check if
     * they are not digits - if not - return false*/
    for(; *str != '\0'; str++){
        if(!isdigit(*str)){
            return false;
        }
    }
    return true;
}

char num_to_base_64(int val){
    char tmp;
    /* capital letters */
    if((tmp = (val & 63)) <= 25){
        tmp += 65;
    }
    /* small letters */
    else if((tmp = (val & 63)) <= 51){
        tmp += 71;
    }
    /* numbers */
    else if((tmp = (val & 63)) <= 61){
        tmp -= 4;
    }
    /* signs */
    else{
        /* + sign */
        if((tmp = (val & 63)) == 62){
            tmp = 43;
        }
        /* / sign */
        else{
            tmp = 47;
        }
    }
    return tmp;
}

void print_error(line_content *line_c, char *format, ...){
    /* define the error print settings, such as added
     * words and colors */
    va_list args;
    va_start(args, format);
    printf("\033[0;31m"); /* set text to red color */
    printf("ERROR: ");
    printf("\033[0m"); /* reset text color */
    vprintf(format, args);
    printf(" in file: ");
    printf("\033[0;34m"); /* set text to blue color */
    printf("%s", line_c->file_name);
    printf("\033[0m"); /* reset text color */
    printf(" in line: ");
    printf("\033[0;33m"); /* set text to yellow color */
    printf("%d\n", line_c->line_number);
    printf("\033[0m"); /* reset text color */
    va_end(args);
}

void update_data_sym_address(symbol_table *s_table, unsigned int ic){
    int i;
    symbol *sym;
    /* go over symbol table*/
    for(i = 0; i < TABLE_SIZE; ++i){
        sym = s_table->symbols[i];
        if(sym == NULL){
            break;
        }
        /* if the symbol is data symbol type - update
         * it's address to the address + ic */
        if(sym->symbol_t == data_symbol){
            sym->symbol_address += ic;
        }
    }
}
