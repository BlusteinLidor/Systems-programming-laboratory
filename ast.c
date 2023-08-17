

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "global_att.h"
#include "ast.h"
#include "other_functions.h"
#define BASE_TEN 10
#define DATA_LENGTH 4
#define STRING_LENGTH 6
#define ENTRY_LENGTH 5
#define EXTERN_LENGTH 6
#define BITS_TO_MOVE 11

/* processes an instruction line and saves it into the tree */
void get_inst(char *line_content, int *index, ast *as_tree){
    int len, i;
    char *comm = NULL;
    op_code op_c;
    /* the op codes number and name */
    op_code cmds[] = {
            {op_mov, "mov"},
            {op_cmp, "cmp"},
            {op_add, "add"},
            {op_sub, "sub"},
            {op_not, "not"},
            {op_clr, "clr"},
            {op_lea, "lea"},
            {op_inc, "inc"},
            {op_dec, "dec"},
            {op_jmp, "jmp"},
            {op_bne, "bne"},
            {op_red, "red"},
            {op_prn, "prn"},
            {op_jsr, "jsr"},
            {op_rts, "rts"},
            {op_stop, "stop"}
    };
    /* get the instruction's length */
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != '\n'
                 && line_content[*index + len] != EOF && !(isspace(line_content[*index + len])); len++);
    /* allocate memory */
    comm = malloc(sizeof(char) * len + 1);
    /* copy the instruction into comm */
    strncpy(comm, line_content + *index, len);
    /* end string with '\0' */
    comm[len] = '\0';
    /* increment index by the length of the instruction */
    (*index) += len;
    /* mark the line as instruction line */
    as_tree->ast_line_option = ast_instruction;
    /* check if the op code is valid by looping through all
     * op code names */
    for(i = 0; i < OP_CODES_NUM; i++){
        op_c = cmds[i];
        if(strcmp(comm, op_c.name) == 0){
            as_tree->ast_dir_or_inst.instruction.op_code.name = op_c.name;
            as_tree->ast_dir_or_inst.instruction.op_code.op_c = op_c.op_c;
            return;
        }
    }
    /* if not valid - mark line as error line and make
     * the error string point it */
    as_tree->ast_line_option = ast_error_line;
    strcpy(as_tree->ast_error, "No such cmd");
}

/* processes a string data and saves it into the tree */
void get_string(char *line_content, int *index, ast *as_tree){
    char *closing_quote;
    int string_len;
    SKIP_WHITE_CHAR(line_content, *index)
    /* if the line is empty - mark as an error line */
    if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String inst doesn't have a string");
        return;
    }
    /* if the string doesn't start with '"', mark as an error line */
    if(line_content[*index] != '"'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String inst doesn't start with quotes");
        return;
    }
    /* go to the string's first character */
    (*index)++;
    closing_quote = strchr(line_content + (*index), '"');
    /* if the closing quote of the string wasn't found - mark line as error */
    if(closing_quote == NULL){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String inst doesn't end with quotes");
        return;
    }
    /* else - save the string in the tree */
    else{
        string_len = closing_quote - (line_content + (*index));
        strncpy(as_tree->ast_dir_or_inst.directive.dir.str, line_content + (*index), string_len);
        as_tree->ast_dir_or_inst.directive.dir.str[string_len] = '\0';
        (*index) += string_len + 1;
    }
    SKIP_WHITE_CHAR(line_content, *index)
    /* if there are additional characters after string - mark as an error */
    if(line_content[*index] != '\n' && line_content[*index] != EOF && line_content[*index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excess characters after string end");
        return;
    }
}
/* processes a .data and saves it into the tree */
void get_data(char *line_content, int *index, ast *as_tree){
    char *num_end;
    int val;
    int num_count = 0;
    /* loop through data numbers as long as we didn't reach end of line */
    do{
        SKIP_WHITE_CHAR(line_content, *index)
        /* if the first character is ',' - mark as an error line */
        if(line_content[*index] == ','){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Line starts with a comma, instead of a number");
            return;
        }
        /* if line is empty - error line */
        else if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Data has to have at least 1 number");
            return;
        }
        /* get first number */
        val = strtol(line_content+ (*index), &num_end, BASE_TEN);
        /* if a number wasn't found - mark as an error */
        if(num_end == line_content + (*index)){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Not a number");
            return;
        }
        /* if the number is out of bounds - error */
        if(val < MIN_INT_SIZE || val > MAX_INT_SIZE){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "The number is out of range");
            return;
        }
        /* add number to number array in tree */
        as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr[num_count] = val;
        /* increment array size */
        as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr_size++;
        /* increment to the next index */
        num_count++;
        /* go to the next character */
        (*index) += num_end - (line_content + (*index));
        SKIP_WHITE_CHAR(line_content, *index)
        /* of the line ended - return */
        if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
            return;
        }
        /* if there isn't a comma between the 2 numbers - error */
        else{
            if(line_content[*index] != ','){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "The character should be ','");
                return;
            }
            /* skip comma */
            (*index)++;
            SKIP_WHITE_CHAR(line_content, *index)
            /* line end - error */
            if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "End of line");
                return;
            }
            /* if there is another comma - error */
            else if(line_content[*index] == ','){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "Excessive commas found");
                return;
            }
        }
    }
    while(*num_end != '\n' && *num_end != EOF && *num_end != '\0');
}

/* processes a .extern and saves it into the tree */
void get_extern(char *line_content, int *index, ast *as_tree){
    char *tmp = NULL;
    int len;
    SKIP_WHITE_CHAR(line_content, *index)
    /* if a label is already defined - ignore it */
    if(as_tree->label[0] != '\0'){
        as_tree->label[0] = '\0';
    }
    /* get the length */
    for(len = 0; line_content[*index + len] != '\n' && line_content[*index + len] != EOF
                 && line_content[*index + len] != '\0' && !(isspace(line_content[*index + len])); len++);
    /* allocate memory */
    tmp = (char *)malloc(sizeof(char) * len + 1);
    /* get the extern label */
    strncpy(tmp, line_content + (*index), len);
    tmp[len] = '\0';
    (*index) += len;

    /* if the label is not valid - mark error and return*/
    if(!is_label(tmp)){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid label");
        free(tmp);
        return;
    }
    /* else - add it to the tree */
    strcpy(as_tree->ast_dir_or_inst.directive.dir.label, tmp);
    /* free memory */
    free(tmp);
    SKIP_WHITE_CHAR(line_content, *index)
    /* if the line didn't end after the label - error */
    if(line_content[*index] != '\n' && line_content[*index] != EOF
       && line_content[*index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after label name");
        return;
    }
}

/* processes a .entry and saves it into the tree */
void get_entry(char *line_content, int *index, ast *as_tree){
    char *tmp = NULL;
    int len;
    SKIP_WHITE_CHAR(line_content, *index)
    /* if a label is already defined - ignore it */
    if(as_tree->label[0] != '\0'){
        as_tree->label[0] = '\0';
    }
    /* if it's the end of line - error */
    if(line_content[*index] == '\n' || line_content[*index] == EOF
       || line_content[*index] == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Must be named");
        return;
    }
    /* get the length of the label */
    for(len = 0; line_content[*index + len] != '\n' && line_content[*index + len] != EOF
                 && line_content[*index + len] != '\0' && !(isspace(line_content[*index + len])); len++);
    /* allocate memory and copy label */
    tmp = (char *)malloc(sizeof(char) * len + 1);
    strncpy(tmp, line_content + (*index), len);
    tmp[len] = '\0';
    (*index) += len;
    /* if the label is not valid - mark error and return*/
    if(!is_label(tmp)){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid label");
        free(tmp);
        return;
    }
    /* else - add it to the tree */
    strcpy(as_tree->ast_dir_or_inst.directive.dir.label, tmp);
    free(tmp);
    SKIP_WHITE_CHAR(line_content, *index)
    /* if there are excess characters after label - error */
    if(line_content[*index] != '\n' && line_content[*index] != EOF
       && line_content[*index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after label name");
        return;
    }
}

/* get directive from the line and add it to the tree */
void get_dir(char *line_content, int *index, ast *as_tree){
    /* skip the dot */
    (*index)++;
    /* if the directive is .data - add it to the tree */
    if(strncmp(line_content + (*index), "data", DATA_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_data_type;
        (*index) += DATA_LENGTH;
    }
    /* if the directive is .string - add it to the tree */
    else if(strncmp(line_content + (*index), "string", STRING_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_string_type;
        (*index) += STRING_LENGTH;
    }
    /* if the directive is .extern - add it to the tree */
    else if(strncmp(line_content + (*index), "extern", EXTERN_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_extern_type;
        (*index) += EXTERN_LENGTH;
    }
    /* if the directive is .entry - add it to the tree */
    else if(strncmp(line_content + (*index), "entry", ENTRY_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_entry_type;
        (*index) += ENTRY_LENGTH;
    }
    /* else - error */
    else{
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Dir not found");
        return;
    }
}

/* check operand's validity and store it in the tree -
 * return operand type number */
operand_type_num check_op(char *op, ast *as_tree){
    int val;
    char *num_end;
    char *num_val;
    /* if the operand's length is 0 - error */
    if(strlen(op) == 0){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "The operand should have a name");
        return error;
    }
    /* if the operand is immediate - get the value */
    if(all_digit(op) || op[0] == '-'){
        if(all_digit(op)){
            num_val = &op[0];
        }
        else if(all_digit(++op)){
            num_val = &op[0];
        }
        val = strtol(num_val, &num_end, BASE_TEN);
        /* if there are characters which are not a number - error */
        if(*num_end != '\0'){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Not a valid number");
            return error;
        }
        /* check if the number is in bounds - return immediate number type */
        if(val >= -(1 << BITS_TO_MOVE) && val < (1 << BITS_TO_MOVE)){
            return immediate;
        }
        /* else - error - out of bounds */
        else{
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Number is out of bounds");
            return error;
        }
    }
    /* fi the operand is a register - check validity */
    else if(op[REG_AT_SIGN_INDEX] == '@' && strlen(op) == REG_LENGTH && op[REG_R_INDEX] == 'r' && isdigit(op[REG_NUM_INDEX])){
        int register_num = op[REG_NUM_INDEX] - '0';
        if(register_num >= 0 && register_num <= REG_NUM){
            return regstr;
        }
        else{
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Not a valid register number");
            return error;
        }
    }
    /* if the operand is a label - check validity */
    else if(is_label(op)){
        return label;
    }
    /* else - return error type number */
    else{
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid operand");
        return error;
    }
}

/* process group a operands and store them in the tree */
void get_group_a_op(char *line_content, int *index, ast *as_tree){
    int len;
    char *comma;
    char *op;
    /* there must be a comma between 2 group a operands */
    if((comma = strchr(line_content + (*index), ',')) == NULL){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Group A operands should have a comma between them");
        return;
    }
    SKIP_WHITE_CHAR(line_content, *index)
    /* if the first character is a comma - error */
    if(line_content[*index] == ','){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Command should start with an operand, not a comma");
        return;
    }
    /* get length of first operand */
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != EOF && line_content[*index + len] != '\n'
                 && !(isspace(line_content[*index + len])) && line_content[*index + len] != ','; len++);
    /* allocate memory for it and copy it's name into op */
    op = malloc(sizeof(char) * len + 1);
    strncpy(op, line_content + *index, len);
    op[len] = '\0';
    /* check validity and store the operand into the tree */
    as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] = check_op(op, as_tree);
    /* if error line - free op and return */
    if(as_tree->ast_line_option == ast_error_line){
        strcpy(as_tree->ast_error, "The first operand is not legal");
        free(op);
        return;
    }
    /* else - check which types are the operands and save their values  */
    else{
        /* first operand - immediate */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == immediate){
            as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].immediate = atoi(op + 1);
        }
            /* first operand - label */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == label){
            strcpy(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].label, op);
        }
            /* first operand - register */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[0] == regstr){
            as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[0].reg = op[2];
        }
    }
    /* free op memory */
    free(op);
    /* get to the end of the first operand */
    (*index) += len;
    SKIP_WHITE_CHAR(line_content, *index)
    /* go to the second operand */
    (*index)++;
    SKIP_WHITE_CHAR(line_content, *index)
    /* if there is an excess comma - error */
    if(line_content[*index] == ','){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excessive commas found");
        return;
    }
    /* get operand's length and allocate memory for it */
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != EOF && line_content[*index + len] != '\n'
                 && !(isspace(line_content[*index + len])) && line_content[*index + len] != ','; len++);
    op = malloc(sizeof(char) * len + 1);
    strncpy(op, line_content + *index, len);
    op[len] = '\0';
    /* check validity and store the operand into the tree */
    as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] = check_op(op, as_tree);
    /* error in the operand's validity */
    if(as_tree->ast_line_option == ast_error_line){
        strcpy(as_tree->ast_error, "The second operand is not legal");
        free(op);
        return;
    }
    else{
        /* second operand - immediate */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == immediate){
            as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].immediate = atoi(op + 1);
        }
        /* second operand - label */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == label){
            strcpy(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].label, op);
        }
        /* second operand - register */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_num_arr[1] == regstr){
            as_tree->ast_dir_or_inst.instruction.op_code_set.a_set_op_codes.inst_arr[1].reg = op[2];
        }
    }
    free(op);
    (*index) += len;
    SKIP_WHITE_CHAR(line_content, *index)
    /* if there are excess characters after operands - error */
    if(line_content[*index] != '\0' && line_content[*index] != EOF && line_content[*index] != '\n'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excess characters after second op");
        return;
    }
}

/* process group b operand and store it in the tree */
void get_group_b_op(char *line_c, int *index, ast *as_tree){
    int len;
    char *op;

    SKIP_WHITE_CHAR(line_c, *index)
    /* shouldn't be a comma because group b has only one operand */
    if(line_c[*index] == ','){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Command should start with an operand, not a comma");
        return;
    }
    /* get operand's length, allocate memory and copy it  */
    for(len = 0; line_c[*index + len] != '\0' && line_c[*index + len] != EOF && line_c[*index + len] != '\n'
                 && !(isspace(line_c[*index + len])); len++);
    op = malloc(sizeof(char) * len + 1);
    strncpy(op, line_c + *index, len);
    op[len] = '\0';
    (*index) += len;
    SKIP_WHITE_CHAR(line_c, *index)
    /* if end of line - error */
    if(line_c[*index] != '\0' && line_c[*index] != EOF && line_c[*index] != '\n'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after operand");
        free(op);
        return;
    }
    /* check validity and store the operand into the tree */
    as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num = check_op(op, as_tree);
    /* if not valid - error */
    if(as_tree->ast_line_option == ast_error_line){
        free(op);
        return;
    }
    else{
        /* operand - immediate */
        if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == immediate){
            as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.immediate = atoi(op);
        }
        /* operand - label */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == label){
            strcpy(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.label, op);
        }
        /* operand - register */
        else if(as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_num == regstr){
            as_tree->ast_dir_or_inst.instruction.op_code_set.b_set_op_codes.inst_arr.reg = op[2];
        }
        /* free memory and return */
        free(op);
        return;
    }
}

ast line_to_ast(char *line_c){
    int i = 0, index = 0;
    ast as_tree = {0};
    char *pointer, *tmp = NULL;
    SKIP_WHITE_CHAR(line_c, i)
    pointer = (char *)(line_c + i);
    /* empty line */
    if(line_c[i] == '\0' || line_c[i] == EOF || line_c[i] == '\n'){
        as_tree.ast_line_option = ast_empty_line;
        return as_tree;
    }
    /* comment line */
    else if(line_c[i] == ';'){
        as_tree.ast_line_option = ast_comment_line;
        return as_tree;
    }
    SKIP_WHITE_CHAR(line_c, i)
    /* label declaration */
    if((pointer = strchr(line_c, ':')) != NULL){
        index = pointer - (line_c + i);
        tmp = malloc(sizeof(char) * index + 1);
        strncpy(tmp, line_c + i, index);
        tmp[index] = '\0';
        /* if label is valid - store it into tree */
        if(is_label(tmp)){
            strcpy(as_tree.label, tmp);
            free(tmp);
            index += (i + 1);
        }
        /* else - error */
        else{
            as_tree.ast_line_option = ast_error_line;
            strcpy(as_tree.ast_error, "Label is not valid");
            free(tmp);
            return as_tree;
        }
    }
    /* if not label declaration - mark no label */
    else{
        as_tree.label[0] = '\0';
    }

    SKIP_WHITE_CHAR(line_c, index)
    /* if directive declaration */
    if(line_c[index] == '.'){
        /* process directive */
        get_dir(line_c, &index, &as_tree);
        SKIP_WHITE_CHAR(line_c, index)
        /* error */
        if(as_tree.ast_line_option == ast_error_line){
            return as_tree;
        }
        /* .string */
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_string_type){
            get_string(line_c, &index, &as_tree);
            return as_tree;
        }
            /* .data */
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_data_type){
            get_data(line_c, &index, &as_tree);
        }
            /* .entry */
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_entry_type){
            get_entry(line_c, &index, &as_tree);
            return as_tree;
        }
            /* .extern */
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_extern_type){
            get_extern(line_c, &index, &as_tree);
            return as_tree;
        }
    }
    /* instruction */
    else{
        get_inst(line_c, &index, &as_tree);
        /* error */
        if(as_tree.ast_line_option == ast_error_line){
            return as_tree;
        }
        SKIP_WHITE_CHAR(line_c, index)
        /* group a op code */
        if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_mov ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_cmp ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_add ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_sub ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_lea){
            get_group_a_op(line_c, &index, &as_tree);
            return as_tree;
        }
        /* group b op code */
        if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_not ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_clr ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_inc ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_dec ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_jmp ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_bne ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_red ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_prn ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_jsr){
            get_group_b_op(line_c, &index, &as_tree);
            return as_tree;
        }
        /* group c op code */
        if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_stop ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_rts){
            SKIP_WHITE_CHAR(line_c, index)
            if(line_c[index] != '\0' && line_c[index] != EOF && line_c[index] != '\n'){
                as_tree.ast_line_option = ast_error_line;
                strcpy(as_tree.ast_error, "There can't be any operands or characters after group c inst");
                return as_tree;
            }
            return as_tree;
        }
    }
    return as_tree;
}


