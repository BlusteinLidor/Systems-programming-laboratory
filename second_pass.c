//
// Created by לידור on 11/06/2023.
//

#include "global_att.h"
#include "ast.h"
#include "other_functions.h"
#include "symbol_table.h"

bool second_pass_process_line(line_content line_c, symbol_type *s_table, ast as_tree){
    symbol *sym;
    if(as_tree.ast_line_option == ast_instruction){
        if(as_tree.ast_dir_or_inst.instruction.inst_name == op_add ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_sub ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_cmp ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_lea ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_mov){
            if(as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == label){
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label);
                    return false;
                }
            }
            else if(as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == label){
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label);
                    return false;
                }
            }
        }
        else if(as_tree.ast_dir_or_inst.instruction.inst_name == op_inc ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_dec ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_not ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_clr ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_prn ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_red){
            if(as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label);
                    return false;
                }
            }
        }
        else if(as_tree.ast_dir_or_inst.instruction.inst_name == op_jsr ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_bne ||
                as_tree.ast_dir_or_inst.instruction.inst_name == op_jmp){
            if(as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label);
                    return false;
                }
            }
            else{
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.b_set_ops.inst_arr.label);
                    return false;
                }
            }
        }
        return true;
    }
    if(as_tree.ast_dir_or_inst.directive.directive_type == dir_entry_type){
        sym = get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.directive.label);
        if(sym == NULL){
            print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.directive.label);
            return false;
        }
        else{
            sym->symbol_t = entry_symbol;
        }
    }
    else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_extern_type){
        sym = get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.directive.label);
        if(sym == NULL){
            print_error(&line_c, "Label %s is not used", as_tree.ast_dir_or_inst.directive.label);
            return false;
        }
        else{
            sym->symbol_t = extern_symbol;
            return true;
        }
    }
    return true;
}

void second_pass_process_label(symbol_table *s_table, code_m_word *code_m, unsigned int ic){
    symbol *sym;
    int i;
    for(i = 0; i < ic; i++){
        if(code_m[i].label != NULL){
            sym = get_symbol_from_table(s_table, code_m[i].label);
            if(sym == NULL){
                continue;
            }
            if(sym->symbol_t == extern_symbol){
                code_m[i].c_word.im_dir = (im_or_dir_m_word *)calloc(1, sizeof(im_or_dir_m_word));
                code_m[i].c_word.im_dir->ARE = 1;
                code_m[i].c_word.im_dir->operand = 0;
            }
            else{
                code_m[i].c_word.im_dir = (im_or_dir_m_word *)calloc(1, sizeof(im_or_dir_m_word));
                code_m[i].c_word.im_dir->ARE = 2;
                code_m[i].c_word.im_dir->operand = (sym->symbol_address) + IC_INIT_VALUE;
            }
        }
    }
}