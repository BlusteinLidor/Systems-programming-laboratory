//
// Created by לידור on 11/06/2023.
//


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "globals.h"
#include "ast.h"

void get_dir(){

}

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

void get_string(){

}

void get_data(){

}

void get_extern(){

}

void get_entry(){

}

void get_operand(){

}