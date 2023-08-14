/* This file contains all the global attributes for the project,
 * like constants, definitions and more */

#ifndef FINAL_PROJECT_GLOBAL_ATT_H
#define FINAL_PROJECT_GLOBAL_ATT_H

#define MAX_LINE_LENGTH 81
#define MEMORY_SIZE 1024
#define MAX_INT_SIZE 4095
#define MIN_INT_SIZE (-4096)
#define IC_INIT_VALUE 100
#define MAX_ERROR_LINE_SIZE 201
#define MAX_LABEL_SIZE 31
#define MAX_MACRO_NAME_SIZE 31
#define TABLE_SIZE 256
#define MAX_NUM_ARR_SIZE 81
#define FAIL 0
#define SUCCESS 1
#define OP_CODES_NUM 16
#define REG_LENGTH 3
#define REG_NUM_INDEX 2
#define REG_R_INDEX 1
#define REG_AT_SIGN_INDEX 0
#define REG_NUM 7

/* machine word of an immediate or direct code */
typedef struct im_or_dir_m_word{
    unsigned int ARE: 2;
    unsigned int operand: 10;
} im_or_dir_m_word;

/* machine word of a .data or .string code */
typedef struct data_m_word{
    unsigned int data_or_string: 12;
} data_m_word;

/* machine word of direct register operands code */
typedef struct im_reg_m_word{
    unsigned int ARE: 2;
    unsigned int target_reg: 5;
    unsigned int source_reg: 5;
} im_reg_m_word;

/* machine word of first word */
typedef struct first_word{
    unsigned int ARE: 2;
    unsigned int target_reg: 3;
    unsigned int operand: 4;
    unsigned int source_reg: 3;
} first_word;

/* defines the registers and an option for not-register */
typedef enum registers{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    NOT_REG = -1
} reg;

/* defines the opcodes and an error */
typedef enum op{
    op_mov,
    op_cmp,
    op_add,
    op_sub,
    op_not,
    op_clr,
    op_lea,
    op_inc,
    op_dec,
    op_jmp,
    op_bne,
    op_red,
    op_prn,
    op_jsr,
    op_rts,
    op_stop,
    op_error
} op;

/* defines an op code - conatines:
 * op_c - the op code number
 * name - the op code name (string) */
typedef struct op_code{
    op op_c;
    char *name;
} op_code;

/* defines a machine word - contains:
 * label - the label name
 * c_word - union holding one of the machine code types:
 *      f_word - first word
 *      im_dir - immediate/direct word
 *      im_reg - register word */
typedef struct code_m_word{
    char *label;
    union c_word{
        first_word *f_word;
        im_or_dir_m_word *im_dir;
        im_reg_m_word *im_reg;
    } c_word;
} code_m_word;

/* defines the symbol options */
typedef enum symbol_type{
    code_symbol,
    data_symbol,
    entry_symbol,
    extern_symbol,
    error_symbol
} symbol_type;

/* defines a line's content:
 *      file_name - the file name the line's in
 *      content - the line's content
 *      line_number - the current line number */
typedef struct line_content{
    char *file_name;
    char *content;
    int line_number;
} line_content;

#endif /*FINAL_PROJECT_GLOBAL_ATT_H*/

