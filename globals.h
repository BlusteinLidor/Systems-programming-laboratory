//
// Created by לידור on 07/06/2023.
//
//@TODO Check if ifndef needed
#ifndef FINAL_PROJECT_GLOBALS_H
#define FINAL_PROJECT_GLOBALS_H

#define MAX_LINE_LENGTH 80
#define MEMORY_SIZE 1024
#define IC_INIT_VALUE 100
#define LABEL_MAX_SIZE 31

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
typedef enum op_code{
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
} op_code;

/* defines the symbol options */
typedef enum symbol{
    ENTRY_SYMBOL,
    DATA_SYMBOL,
    EXTERN_SYMBOL,
    CODE_SYMBOL
} symbol;

typedef struct line_content{
    char *filename;
    char *content;
    int line_number;
} line_content;





#endif //FINAL_PROJECT_GLOBALS_H
