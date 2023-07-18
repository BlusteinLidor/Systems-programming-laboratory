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


void get_inst(char *line_content, int *index, ast *as_tree){
    int len;
    char *comm = NULL;
    typedef struct cmd{
        char *inst_name;
        op_code inst_code;
    } cmd;
    cmd cmds[] = {
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
            {"stop", op_stop},
    };
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != '\n'
    && line_content[*index + len] != EOF && !(isspace(line_content[*index + len])); len++);
    comm = malloc(sizeof(char) * len);
    strncpy(comm, line_content + *index, len);
    comm[len] = '\0';
    (*index) += len;

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
    while(*num_end != '\n' || *num_end != EOF || *num_end != '\0');
}

void get_extern(){

}

void get_entry(){

}

void get_operand(){

}

void get_dir(){

}