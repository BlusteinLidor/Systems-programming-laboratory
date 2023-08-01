//
// Created by לידור on 01/08/2023.
//

#include "inst_process.h"
#include <stdlib.h>
#include "other_functions.h"
#include "ast.h"
#include "global_att.h"

im_or_dir_m_word int_to_bits(int num){
    im_or_dir_m_word w;
    if(num < 0){
        unsigned int pos_val = abs(num);
        unsigned int comp_val = (~pos_val) + 1;
        w.operand = (1 << 11) | (comp_val & 0xFFF);
    }
    else{
        w.operand = num & 0xFFF;
    }
    return w;
}

void process_immediate(int op, int *ic, code_m_word code_m[]){
    code_m_word code_word = {0};
    code_word.label = NULL;
    code_word.c_word.im_dir = (im_or_dir_m_word *) calloc(1, sizeof(im_or_dir_m_word));
    *(code_word.c_word.im_dir) = int_to_bits(op);
    code_word.c_word.im_dir->ARE = 0;
    code_m[(*ic)++] = code_word;
}

void process_register(int *ic, code_m_word code_m[], char reg1, char reg2){
    code_m_word code_word;
    code_word.label = NULL;
    code_word.c_word.im_reg = (im_reg_m_word *)calloc(1, sizeof(im_reg_m_word));
    if(reg1 >= 0){
        code_word.c_word.im_reg->source_reg = reg1 - '0';
    }
    if(reg2 >= 0){
        code_word.c_word.im_reg->target_reg = reg2 - '0';
    }
    code_m[(*ic)++] = code_word;
}

void process_label(int *ic, code_m_word code_m[], char label[]){
    code_m_word code_word;
    int len;
    for(len = 0; label[len] != '\0'; len++){
        code_word.label = calloc(len + 1, sizeof(char));
        code_word.label[len] = '\0';
        strcpy(code_word.label, label);
        code_m[(*ic)++] = code_word;
    }
}

bool process_inst(line_content *line_c, unsigned int *ic, symbol_table *s_table, code_m_word *code_m, ast *as_tree){
    /* @TODO proceed */
}

