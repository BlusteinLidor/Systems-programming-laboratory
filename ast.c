//
// Created by לידור on 11/06/2023.
//


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "global_att.h"
#include "ast.h"
#include "other_functions.h"
#define BASE_TEN 10
#define DATA_LENGTH 4
#define STRING_LENGTH 6
#define ENTRY_LENGTH 5
#define EXTERN_LENGTH 6

/* @TODO make index a pointer (*index) */


void get_inst(char *line_content, ast *as_tree){
    int index = 0;
    int len;
    char *comm = NULL;
    op_code_l code_l[] = {
            {"mov", op_mov},
            {"cmp", op_cmp},
            {"add", op_add},
            {"sub", op_sub},
            {"not", op_not},
            {"clr", op_clr},
            {"lea", op_lea},
            {"inc", op_inc},
            {"dec", op_dec},
            {"jmp", op_jmp},
            {"bne", op_bne},
            {"red", op_red},
            {"prn", op_prn},
            {"jsr", op_jsr},
            {"rts", op_rts},
            {"stop", op_stop}
    };
    for(len = 0; line_content[index + len] != '\0' && line_content[index + len] != '\n'
    && line_content[index + len] != EOF && !(isspace(line_content[index + len])); len++);
    comm = malloc(sizeof(char) * len);
    strncpy(comm, line_content + index, len);
    comm[len] = '\0';
    (index) += len;

    as_tree->ast_line_option = ast_instruction;

}

void get_string(char *line_content, ast *as_tree){
    char *closing_quote;
    int index = 0;
    index = skip_white_char(line_content, index);
    if(line_content[index] == '\n' || line_content[index] == EOF || line_content[index] == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String inst doesn't have a string");
        return;
    }
    if(line_content[index] != '"'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String inst doesn't start with quotes");
        return;
    }
    index++;
    closing_quote = strchr(line_content+ index, '"');
    if(closing_quote == NULL){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String inst doesn't end with quotes");
        return;
    }
    else{
        int string_len = closing_quote - (line_content + index);
        strncpy(as_tree->ast_dir_or_inst.directive.str, line_content + index, string_len);
        as_tree->ast_dir_or_inst.directive.str[string_len] = '\0';
        index += string_len + 1;
    }
    index = skip_white_char(line_content, index);
    if(line_content[index] == '\n' || line_content[index] == EOF || line_content[index] == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excess characters after string end");
        return;
    }
}

void get_data(char *line_content, ast *as_tree){
    char *num_end;
    int val = 0;
    int num_count = 0;
    int index = 0;

    do{
        index = skip_white_char(line_content, index);
        if(line_content[index] == ','){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Line starts with a comma, instead of a number");
            return;
        }
        else if(line_content[index] == '\n' || line_content[index] == EOF || line_content[index] == '\0'){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Data has to have at least 1 number");
            return;
        }
        val = strtol(line_content+index, &num_end, BASE_TEN);
        if(num_end == line_content + index){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Not a number");
            return;
        }
        if(val < MIN_INT_SIZE || val > MAX_INT_SIZE){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "The number is out of range");
            return;
        }
        as_tree->ast_dir_or_inst.directive.num_arr.int_arr[num_count] = val;
        as_tree->ast_dir_or_inst.directive.num_arr.int_arr_size++;
        num_count++;
        index = skip_white_char(line_content, (num_end - (line_content + index)));
        if(line_content[index] == '\n' || line_content[index] == EOF || line_content[index] == '\0'){
            return;
        }
        else{
            if(line_content[index] != ','){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "The character should be ','");
                return;
            }
            index++;
            index = skip_white_char(line_content, index);
            if(line_content[index] == '\n' || line_content[index] == EOF || line_content[index] == '\0'){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "End of line");
                return;
            }
            else if(line_content[index] == ','){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "Excessive commas found");
                return;
            }
        }
    }
    while(*num_end != '\n' && *num_end != EOF && *num_end != '\0');
}

void get_extern(char *line_content, ast *as_tree){
    char *tmp = NULL;
    int len = 0;
    int index = 0;
    index = skip_white_char(line_content, index);
    if(as_tree->label[0] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Name already used");
        return;
    }
    for(len = 0; line_content[index + len] != '\n' && line_content[index + len] != EOF
    && line_content[index + len] != '\0' && !(isspace(line_content[index + len])); len++);
    tmp = (char *)malloc(sizeof(char) * len);
    strncpy(tmp, line_content + index, len);
    tmp[len] = '\0';
    index += len;

    if(!is_label(tmp)){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid label");
        free(tmp);
        return;
    }
    strcpy(as_tree->ast_dir_or_inst.directive.label, tmp);
    free(tmp);
    index = skip_white_char(line_content, index);
    if(line_content[index] != '\n' && line_content[index] != EOF
       && line_content[index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after label name");
        return;
    }
}

void get_entry(char *line_content, ast *as_tree){
    char *tmp = NULL;
    int len = 0;
    int index = 0;
    index = skip_white_char(line_content, index);
    if(as_tree->label[0] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Name already used in label");
        return;
    }
    if(line_content + index == '\n' || line_content + index == EOF
    || line_content + index == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Must be named");
        return;
    }
    for(len = 0; line_content[index + len] != '\n' && line_content[index + len] != EOF
                 && line_content[index + len] != '\0' && !(isspace(line_content[index + len])); len++);
    tmp = (char *)malloc(sizeof(char) * len);
    strncpy(tmp, line_content + index, len);
    tmp[len] = '\0';
    index += len;

    if(!is_label(tmp)){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid label");
        free(tmp);
        return;
    }
    strcpy(as_tree->ast_dir_or_inst.directive.label, tmp);
    free(tmp);
    index = skip_white_char(line_content, index);
    if(line_content[index] != '\n' && line_content[index] != EOF
       && line_content[index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after label name");
        return;
    }
}

void get_opcode(char *line_content, ast *as_tree){
    char *cmd = NULL;
    int len = 0;
    int index = 0;
    op_code_l code_l[] = {
            {"mov", op_mov},
            {"cmp", op_cmp},
            {"add", op_add},
            {"sub", op_sub},
            {"not", op_not},
            {"clr", op_clr},
            {"lea", op_lea},
            {"inc", op_inc},
            {"dec", op_dec},
            {"jmp", op_jmp},
            {"bne", op_bne},
            {"red", op_red},
            {"prn", op_prn},
            {"jsr", op_jsr},
            {"rts", op_rts},
            {"stop", op_stop}
    };
    for(len = 0; line_content[index + len] != '\0' && line_content[index + len] != '\n'
                 && line_content[index + len] != EOF && !(isspace(line_content[index + len])); len++);
    cmd = malloc(sizeof(char) * len);
    strncpy(cmd, line_content + index, len);
    index += len;
}

void get_dir(char *line_content, ast *as_tree){
    int index = 1;
    if(strncmp(line_content + index, "data", DATA_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_data_type;
        index += DATA_LENGTH;
    }
    else if(strncmp(line_content + index, "string", STRING_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_string_type;
        index += STRING_LENGTH;
    }
    else if(strncmp(line_content + index, "extern", EXTERN_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_extern_type;
        index += EXTERN_LENGTH;
    }
    else if(strncmp(line_content + index, "entry", ENTRY_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_entry_type;
        index += ENTRY_LENGTH;
    }
    else{
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Dir not found");
        return;
    }
}