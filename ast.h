/* This file underlines the Abstract syntax tree for the
 * project - this tree is a representation of the
 * abstract syntactic structure of text of each line -
 * an easier way to organize and save information and parse */

#ifndef FINAL_PROJECT_AST_H
#define FINAL_PROJECT_AST_H

#include "global_att.h"

/* operand type values - immediate number/label string/register number */
union operand_type{
    int immediate;
    char label[MAX_LABEL_SIZE];
    int reg;
};

/* operand type number - error/immediate/label/register */
typedef enum{
	error = -1,
    immediate = 1,
    label = 3,
    regstr = 5
} operand_type_num;

/* The Abstract syntax tree */
typedef struct ast{
    /* the line option type */
    enum{
        ast_empty_line,
        ast_comment_line,
        ast_directive,
        ast_instruction,
        ast_error_line
    } ast_line_option;
    /* label string */
    char label[MAX_LABEL_SIZE];
    /* error string */
    char ast_error[MAX_ERROR_LINE_SIZE];
    /* directive or instruction */
    union{
        /* directive */
        struct {
            /* directive type */
            enum{
                dir_data_type,
                dir_string_type,
                dir_entry_type,
                dir_extern_type
            } directive_type;
            /* directive content */
            union{
                /* .data */
                struct{
                    /* int array */
                    int int_arr[MAX_NUM_ARR_SIZE];
                    /* array size */
                    int int_arr_size;
                } num_arr;
                /* .string */
                char str[MAX_LINE_LENGTH + 1];
                /* label */
                char label[MAX_LABEL_SIZE + 1];
            } dir;
        } directive;
        /* instruction */
        struct{
            /* op code information */
            op_code op_code;
            /* op code sets */
            union{
                /* op code a set */
                struct{
                    /* operand type number array */
                    operand_type_num inst_num_arr[2];
                    /* operand type values array */
                    union operand_type inst_arr[2];
                } a_set_op_codes;
                /* op code b set */
                struct{
                    /* operand type number */
                    operand_type_num op_type_num;
                    /* operand type value */
                    union operand_type op_value;
                } b_set_op_codes;
            } op_code_set;
        } instruction;
    } ast_dir_or_inst;
} ast;

/* makes an Abstract syntax tree from a line
 * @param line_c - current line
 * @return ast - the tree made from the line */
ast line_to_ast(char *line_c);

#endif
