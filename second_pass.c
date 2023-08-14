#include "global_att.h"
#include "ast.h"
#include "other_functions.h"
#include "symbol_table.h"
#define ARE_EXTERN 1
#define ARE_SYM_IN_FILE 2


bool second_pass_process_line(line_content line_c, symbol_table *s_table, ast as_tree){
    symbol *sym;
    /* if it's an instruction line */
    if(as_tree.ast_line_option == ast_instruction){
        /* if it's first group op code */
        if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_add ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_sub ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_cmp ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_lea ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_mov){
            /* if the operand is a label */
            if(as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == label){
                /* if the label is not defined - print error and return false */
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label);
                    return false;
                }
            }
            /* if the second operand is a label */
            else if(as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == label){
                /* if the label is not defined - print error and return false */
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label);
                    return false;
                }
            }
        }
        /* if it's second group op code */
        else if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_inc ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_dec ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_not ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_clr ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_prn ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_red ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_jsr ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_bne ||
                as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_jmp){
            /* if the operand is a label */
            if(as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
                /* if the label is not defined - print error and return false */
                if(get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.label) == NULL){
                    print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.label);
                    return false;
                }
            }
        }
        /* the label is defined - return true */
        return true;
    }
    /* if the directive is entry */
    if(as_tree.ast_dir_or_inst.directive.directive_type == dir_entry_type){
        /* get the symbol by the label */
        sym = get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.directive.dir.label);
        /* if label is not defined - print error and return false */
        if(sym == NULL){
            print_error(&line_c, "Label %s is not defined", as_tree.ast_dir_or_inst.directive.dir.label);
            return false;
        }
        /* else - mark the symbol as entry type */
        else{
            sym->symbol_t = entry_symbol;
        }
    }
    /* if the directive is extern */
    else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_extern_type){
        /* get the symbol by the label */
        sym = get_symbol_from_table(s_table, as_tree.ast_dir_or_inst.directive.dir.label);
        /* if label is not defined - print error and return false */
        if(sym == NULL){
            print_error(&line_c, "Label %s is not used", as_tree.ast_dir_or_inst.directive.dir.label);

            return false;
        }
        /* else - mark the symbol as extern type */
        else{
            sym->symbol_t = extern_symbol;
        }
    }
    return true;
}

void second_pass_process_label(symbol_table *s_table, code_m_word *code_m, unsigned int ic){
    symbol *sym;
    int i;
    /* loop over the machine code table */
    for(i = 0; i < ic; i++){
        /* if the label is defined, get the symbol from the symbol table*/
        if(code_m[i].label != NULL){
            sym = get_symbol_from_table(s_table, code_m[i].label);
            /* if the symbol wasn't found (null) - continue */
            if(sym == NULL){
                continue;
            }
            /* if the symbol type is extern - process the machine code accordingly */
            if(sym->symbol_t == extern_symbol){
                code_m[i].c_word.im_dir = (im_or_dir_m_word *)calloc(1, sizeof(im_or_dir_m_word));
                code_m[i].c_word.im_dir->ARE = ARE_EXTERN;
                code_m[i].c_word.im_dir->operand = 0;
            }
                /* else - process the machine code accordingly */
            else{
                code_m[i].c_word.im_dir = (im_or_dir_m_word *)calloc(1, sizeof(im_or_dir_m_word));
                code_m[i].c_word.im_dir->ARE = ARE_SYM_IN_FILE;
                code_m[i].c_word.im_dir->operand = (sym->symbol_address) + IC_INIT_VALUE;
            }
        }
    }
}
