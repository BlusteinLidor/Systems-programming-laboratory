//
// Created by לידור on 19/06/2023.
//

#ifndef FINAL_PROJECT_AST_H
#define FINAL_PROJECT_AST_H

#include "global_att.h"

union operand_type{
    int immediate;
    char label[MAX_LABEL_SIZE];
    char reg;
};

typedef enum{
    immediate,
    label,
    regstr,
    error
} operand_type_num;

/*typedef enum{
    inst_mov,
    inst_cmp,
    inst_add,
    inst_sub,
    inst_lea
} a_inst_set;
typedef enum{
    inst_not,
    inst_clr,
    inst_inc,
    inst_dec,
    inst_jmp,
    inst_bne,
    inst_red,
    inst_prn,
    inst_jsr
} b_inst_set;
typedef enum{
    inst_rts,
    inst_stop
} c_inst_set;*/

typedef struct ast{
    enum{
        ast_empty_line,
        ast_comment_line,
        ast_directive,
        ast_instruction,
        ast_error_line
    } ast_line_option;

    char label[MAX_LABEL_SIZE];
    char ast_error[MAX_ERROR_LINE_SIZE];

    union{
        struct {
            enum{
                dir_data_type,
                dir_string_type,
                dir_entry_type,
                dir_extern_type
            } directive_type;

            union{
                struct{
                    int int_arr[MAX_NUM_ARR_SIZE];
                    int int_arr_size;
                } num_arr;
                char str[MAX_LINE_LENGTH];
                char label[MAX_LABEL_SIZE];
            };
        } directive;

        struct{
            op_code inst_name;
            union{
                struct{
                    operand_type_num inst_num_arr[2];
                    union operand_type inst_arr[2];
                } a_set_op_codes;
                struct{
                    operand_type_num inst_num;
                    union{
                        struct{
                            char label[MAX_LABEL_SIZE + 1];
                            operand_type_num inst_num_arr[2];
                            union operand_type inst_arr[2];
                        } b_set_label;
                        union operand_type inst_arr;
                    }b_set_ops;
                } b_set_op_codes;
                struct{

                } c_set_op_codes;
            } op_code_set;
        } instruction;
    } ast_dir_or_inst;

} ast;

ast line_to_ast(char *line_c);

#endif //FINAL_PROJECT_AST_H