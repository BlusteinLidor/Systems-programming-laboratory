

#include "inst_process.h"
#include <stdlib.h>
#include "other_functions.h"
#include "ast.h"
#include "global_att.h"

im_or_dir_m_word int_to_bits_inst(int num){
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

void process_immediate(unsigned int *ic, code_m_word code_m[], int op){
    code_m_word code_word = {0};
    code_word.label = NULL;
    code_word.c_word.im_dir = (im_or_dir_m_word *) calloc(1, sizeof(im_or_dir_m_word));
    *(code_word.c_word.im_dir) = int_to_bits_inst(op);
    code_word.c_word.im_dir->ARE = 0;
    code_m[(*ic)++] = code_word;
}

void process_register(unsigned int *ic, code_m_word code_m[], char reg1, char reg2){
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

void process_label(unsigned int *ic, code_m_word code_m[], char label[]){
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
    if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_add ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_sub ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_cmp ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_lea ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_mov){
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == immediate &&
        as_tree->ast_dir_or_inst.instruction.op_code.op_c != op_cmp){
            print_error(line_c, "Not a valid destination for the next cmds: add, sub, mov");
            return false;
        }
        if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_lea){
            if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == immediate ||
            as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr){
                print_error(line_c, "Not a valid source for the cmd lea");
                return false;
            }
            else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == immediate){
                print_error(line_c, "Not a valid destination for the cmd lea");
                return false;
            }
        }
        /* two regs */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr &&
        as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == regstr){
            process_register(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].reg,
                             as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].reg);
            return true;
        }
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == immediate){
            process_immediate(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].immediate);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == label) {
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr) {
            process_register(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].reg, -1);
        }
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == immediate) {
            process_immediate(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].immediate);
            return true;
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == label) {
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label);
            return true;
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == regstr) {
            process_register(ic, code_m, -1, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].reg);
            return true;
        }
    }

    if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_inc ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_dec ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_not ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_clr ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_prn ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_red){
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == immediate &&
        as_tree->ast_dir_or_inst.instruction.op_code.op_c != op_prn){
            as_tree->ast_line_option = ast_error_line;
            print_error(line_c, "Not valid for this cmd");
            return false;
        }
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == immediate){
            process_immediate(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.immediate);
            return true;
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label);
            return true;
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == regstr){
            process_register(ic, code_m, -1, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.reg);
            return true;
        }
    }

    else if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_jsr ||
            as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_bne ||
            as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_jmp){
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == immediate ||
        as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == regstr){
            as_tree->ast_line_option = ast_error_line;
            print_error(line_c, "Not valid for this cmd");
            return false;
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label);
            return true;
        }
    }

    return true;
}

