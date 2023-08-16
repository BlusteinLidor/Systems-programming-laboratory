#include "inst_process.h"
#include <stdlib.h>
#include "other_functions.h"
#include "ast.h"
#include "global_att.h"

/* convert immediate word into machine code */
int imm_to_bits(int num){
    int tmp;
    /* if the number is negative - convert it by using
     * 2's compliment */
    if(num < 0){
        unsigned int pos_val = abs(num);
        unsigned int comp_val = (~pos_val) + 1;
        int comp = (comp_val & 0xFFF);
        tmp = ((1 << 9) | comp);
    }
    else{
        tmp = num & 0xFFF;
    }
    return tmp;
}

void process_first_word(unsigned int *ic, code_m_word code_m[], op_code op,
                        operand_type_num first_op, operand_type_num second_op){
    code_m_word c_word;
    c_word.label = NULL;
    /* allocate memory */
    c_word.c_word.f_word = (first_word *)calloc (1, sizeof(first_word));
    /* op code number */
    c_word.c_word.f_word->operand = op.op_c;
    /* first operand */
    c_word.c_word.f_word->source_reg = first_op;
    /* second operand */
    c_word.c_word.f_word->target_reg = second_op;
    /* save the machine word and increment machine word
     * array pointer */
    code_m[(*ic)++] = c_word;
}

void process_immediate(unsigned int *ic, code_m_word code_m[], int op){
    code_m_word code_word = {0};
    code_word.label = NULL;
    /* allocate memory */
    code_word.c_word.im_dir = (im_or_dir_m_word *) calloc(1, sizeof(im_or_dir_m_word));
    /* immediate number */
    code_word.c_word.im_dir->operand = imm_to_bits(op);
    /* ARE part equals 0 (immediate) */
    code_word.c_word.im_dir->ARE = 0;
    /* save the machine word and increment machine word
     * array pointer */
    code_m[(*ic)++] = code_word;
}

void process_register(unsigned int *ic, code_m_word code_m[], char reg1, char reg2){
    code_m_word code_word;
    code_word.label = NULL;
    /* allocate memory */
    code_word.c_word.im_reg = (im_reg_m_word *)calloc(1, sizeof(im_reg_m_word));
    /* if regs value are valid - save them in machine code */
    if(reg1 >= 0){
        code_word.c_word.im_reg->source_reg = reg1 - '0';
    }
    if(reg2 >= 0){
        code_word.c_word.im_reg->target_reg = reg2 - '0';
    }
}

void process_label(unsigned int *ic, code_m_word code_m[], char label[]){
    code_m_word code_word;
    int len = 0;
    /* get label length */
    while(label[len] != '\0'){
        len++;
    }
    /* allocate memory */
    code_word.label = calloc(len + 1, sizeof(char));
    code_word.label[len] = '\0';
    /* save label in machine word */
    strcpy(code_word.label, label);
    /* save the machine word and increment machine word
     * array pointer */
    code_m[(*ic)++] = code_word;
}

bool process_inst(line_content *line_c, unsigned int *ic, symbol_table *s_table, code_m_word *code_m, ast *as_tree){
	/* first op_code group */
    if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_add ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_sub ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_cmp ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_lea ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_mov){
        /* if the op code is not 'cmp' and the second operand is immediate
         * print error and return false*/
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == immediate &&
           as_tree->ast_dir_or_inst.instruction.op_code.op_c != op_cmp){
            print_error(line_c, "Not a valid destination op for the next cmds: add, sub, mov, lea");
            return false;
        }
        /* if the op code is 'lea' and the first operand is either
         * immediate or register - print error and return false*/
        if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_lea){
            if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == immediate ||
               as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr){
                print_error(line_c, "Not a valid source op for the cmd lea");
                return false;
            }
        }
        /* process the first word */
        process_first_word(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code,
                           as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0],
                           as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1]);
        /* if both operands are registers */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr &&
           as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == regstr){
            process_register(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].reg,
                             as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].reg);
            return true;
        }
        /* first operand is an immediate */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == immediate){
            process_immediate(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].immediate);
        }
        /* first operand is a label */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == label) {
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label);
        }
        /* first operand is a register */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr) {
            process_register(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].reg, -1);
        }
        /* second operand is an immediate */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == immediate) {
            process_immediate(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].immediate);
            return true;
        }
        /* second operand is a label */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == label) {
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label);
            return true;
        }
        /* second operand is a register */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == regstr) {
            process_register(ic, code_m, -1, as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].reg);
            return true;
        }
    }
	/* second op_code group */
    if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_inc ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_dec ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_not ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_clr ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_prn ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_red ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_jsr ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_bne ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_jmp){
        /* if the op code is not 'prn' and the operand is immediate
         * print error and return false*/
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == immediate &&
           as_tree->ast_dir_or_inst.instruction.op_code.op_c != op_prn){
            as_tree->ast_line_option = ast_error_line;
            print_error(line_c, "Not valid for this cmd");
            return false;
        }
        /* process first word */
        process_first_word(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code, 0,
                           as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num);
        /* operand is an immediate */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == immediate){
            process_immediate(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.immediate);
            return true;
        }
        /* operand is a label */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
            process_label(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.label);
            return true;
        }
        /* operand is a register */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == regstr){
            process_register(ic, code_m, -1, as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.reg);
            return true;
        }
    }
	/* third op_code group */
    if(as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_rts ||
       as_tree->ast_dir_or_inst.instruction.op_code.op_c == op_stop){
        process_first_word(ic, code_m, as_tree->ast_dir_or_inst.instruction.op_code, 0, 0);
    }

    return true;
}


