/*
 Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "global_att.h"
#include <stdarg.h>
#include "symbol_table.h"

/* @TODO check the num_to_base_64 function */

int skip_white_char(char *string, int index){
    for(; string[index] && isspace(string[index]); (index)++);
    return index;
}

char *str_cat(char *str1, char *str2){
    char *final_str = (char *) malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(final_str, str1);
    strcat(final_str, str2);
    return final_str;
}

bool is_empty(char c){
    if(c == '\n' || c == '\0' || c == EOF || c == ';'){
        return true;
    }
    return false;
}

int string_num(char *str, char *delim){
    int count = 0;
    char *temp;
    char *copy = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
    strcpy(copy, str);

    temp = strtok(copy, delim);
    while(temp != NULL){
        count++;
        temp = strtok(NULL, delim);
    }

    return count;
}

char **strings(char *string, char *delim){
    int str_num = 0, i = 0;
    char *token;
    char **strings;
    char *copy = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
    char *temp = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
    strcpy(copy, string);
    str_num = string_num(copy, delim);
    strings = malloc(sizeof(char *) * str_num);
    if(strings == NULL){
        free(strings);
        free(temp);
        free(copy);
        return NULL;
    }

    token = strtok(copy, delim);

    while(token != NULL && i < str_num){
        strcpy(temp, token);
        strings[i] = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        if(strings[i] == NULL){
            free(strings[i]);
            free(strings);
            free(temp);
            free(copy);
            return NULL;
        }
        strcpy(strings[i++], temp);
        token = strtok(NULL, delim);
    }
    free(temp);
    free(copy);
    return strings;
}

void free_string_p(char **strings, int string_num){
    int i;
    for(i = 0; i < string_num; i++){
        free(strings[i]);
    }
    free(strings);
}

void free_c_word(code_m_word c_word[], unsigned int ic){
    int i;
    for(i = 0; i < ic; i++){
        if(c_word[i].label != NULL){
            free(c_word[i].label);
            c_word[i].label = NULL;
        }
        if(c_word[i].c_word.im_dir != NULL){
            free(c_word[i].c_word.im_dir);
            c_word[i].c_word.im_dir = NULL;
        }
        if(c_word[i].c_word.im_reg != NULL){
            free(c_word[i].c_word.im_reg);
            c_word[i].c_word.im_reg = NULL;
        }
    }
}

bool is_label(char *str){
    char label[MAX_LABEL_SIZE + 1];
    int i = 0;
    char *not_label_names[] = {"r0", "r1", "r2", "r3", "r4",
                               "r5", "r6", "r7", "and", "mov",
                               "cmp", "add", "sub", "not",
                               "clr", "lea","inc", "dec",
                               "jmp", "bne", "red", "prn",
                               "jsr", "rts", "stop"};
    int nln_len = 25;
    int str_len = strlen(str);
    if(str_len > MAX_LABEL_SIZE){
        return false;
    }
    if(str[str_len - 1] == ':'){
        strncpy(label, str, str_len - 1);
        label[str_len] = '\0';
    }
    else{
        strncpy(label, str, str_len);
        label[str_len] = '\0';
    }
    if(!isalpha(label[0])){
        return false;
    }
    for(i = 0; i < nln_len; i++){
        if(strcmp(label, not_label_names[i]) == 0){
            return false;
        }
    }
    return true;
}

bool all_digit(char *str){
    for(; *str != '\0'; str++){
        if(!isdigit(*str)){
            return false;
        }
    }
    return true;
}

char num_to_base_64(int val){
    char tmp = 0;
    /* capital letters */
    if((val & 63) >= 0 || (val & 63) <= 25){
        tmp = (val & 63) + 65;
    }
        /* small letters */
    else if((val & 63) >= 26 || (val & 63) <= 51){
        tmp = (val & 63) + 71;
    }
        /* numbers */
    else if((val & 63) >= 52 || (val & 63) <= 61){
        tmp = (val & 63) - 4;
    }
        /* signs */
    else{
        /* + sign */
        if((val & 63) == 62){
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
    va_list args;
    va_start(args, format);
    printf("\033[0;31m");
    printf("ERROR: ");
    printf("\033[0m");
    vprintf(format, args);
    printf(" in file: ");
    printf("\033[0;35m");
    printf("%s", line_c->file_name);
    printf("\033[0m");
    printf(" in line: ");
    printf("\033[0;35m");
    printf("%d\n", line_c->line_number);
    printf("\033[0m");
    va_end(args);
}

void update_data_sym_address(symbol_table *s_table, unsigned int ic){
    int i;
    symbol *sym;
    for(i = 0; i < TABLE_SIZE; i++){
        sym = s_table->symbols[i];
        if(sym == NULL){
            break;
        }
        if(sym->symbol_t == data_symbol){
            printf("inside o_f\n");
            sym->symbol_address += ic;
        }
    }
}