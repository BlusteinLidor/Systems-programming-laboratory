

#ifndef FINAL_PROJECT_AST_H
#define FINAL_PROJECT_AST_H

#include "global_att.h"

union operand_type{
    int immediate;
    char label[MAX_LABEL_SIZE];
    int reg;
};

typedef enum{
	error = -1,
    immediate = 1,
    label = 3,
    regstr = 5
} operand_type_num;

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
                char str[MAX_LINE_LENGTH + 1];
                char label[MAX_LABEL_SIZE + 1];
            } dir;
        } directive;

        struct{
            op_code op_code;
            union{
                struct{
                    operand_type_num inst_num_arr[2];
                    union operand_type inst_arr[2];
                } a_set_op_codes;
                struct{
                    operand_type_num inst_num;
                    union operand_type inst_arr;
                } b_set_op_codes;
            } op_code_set;
        } instruction;
    } ast_dir_or_inst;

} ast;

ast line_to_ast(char *line_c);

#endif
