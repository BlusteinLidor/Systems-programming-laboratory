

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


void get_inst(char *line_content, int *index, ast *as_tree){
    int len, i;
    char *comm = NULL;
    op_code op_c;
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
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != '\n'
                 && line_content[*index + len] != EOF && !(isspace(line_content[*index + len])); len++);
    comm = malloc(sizeof(char) * len + 1);
    strncpy(comm, line_content + *index, len);
    comm[len] = '\0';
    (*index) += len;
    as_tree->ast_line_option = ast_instruction;
    for(i = 0; i < OP_CODES_NUM; i++){
        op_c = cmds[i];
        if(strcmp(comm, op_c.name) == 0){
            as_tree->ast_dir_or_inst.instruction.op_code.name = op_c.name;
            as_tree->ast_dir_or_inst.instruction.op_code.op_c = op_c.op_c;
            return;
        }
    }
    as_tree->ast_line_option = ast_error_line;
    strcpy(as_tree->ast_error, "No such cmd");
}

void get_string(char *line_content, int *index, ast *as_tree){
    char *closing_quote;
    int string_len;
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String instruction have no characters");
        return;
    }
    if(line_content[*index] != '"'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String instruction doesn't start with quotes");
        return;
    }
    (*index)++;
    closing_quote = strchr(line_content + (*index), '"');
    if(closing_quote == NULL){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "String instruction doesn't end with quotes");
        return;
    }
    else{
        string_len = closing_quote - (line_content + (*index));
        strncpy(as_tree->ast_dir_or_inst.directive.dir.str, line_content + (*index), string_len);
        as_tree->ast_dir_or_inst.directive.dir.str[string_len] = '\0';
        (*index) += string_len + 1;
    }
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] != '\n' && line_content[*index] != EOF && line_content[*index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excess characters after string end");
        return;
    }
}

void get_data(char *line_content, int *index, ast *as_tree){
    char *num_end;
    int val;
    int num_count = 0;

    do{
        SKIP_WHITE_CHAR(line_content, *index)
        if(line_content[*index] == ','){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Data line starts with a comma, instead of a number");
            return;
        }
        else if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Data has to have at least 1 number");
            return;
        }
        val = strtol(line_content+ (*index), &num_end, BASE_TEN);
        if(num_end == line_content + (*index)){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Data must have numbers only");
            return;
        }
        if(val < MIN_INT_SIZE || val > MAX_INT_SIZE){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "The number is out of range");
            return;
        }
        as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr[num_count] = val;
        as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr_size++;
        num_count++;
        (*index) += num_end - (line_content + (*index));
        SKIP_WHITE_CHAR(line_content, *index)
        if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
            return;
        }
        else{
            if(line_content[*index] != ','){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "The character should be ','");
                return;
            }
            (*index)++;
            SKIP_WHITE_CHAR(line_content, *index)
            if(line_content[*index] == '\n' || line_content[*index] == EOF || line_content[*index] == '\0'){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "End of line");
                return;
            }
            else if(line_content[*index] == ','){
                as_tree->ast_line_option = ast_error_line;
                strcpy(as_tree->ast_error, "Excessive commas found");
                return;
            }
        }
    }
    while(*num_end != '\n' && *num_end != EOF && *num_end != '\0');
}

void get_extern(char *line_content, int *index, ast *as_tree){
    char *tmp = NULL;
    int len;
    SKIP_WHITE_CHAR(line_content, *index)
    if(as_tree->label[0] != '\0'){
		as_tree->label[0] = '\0';
    }
    for(len = 0; line_content[*index + len] != '\n' && line_content[*index + len] != EOF
                 && line_content[*index + len] != '\0' && !(isspace(line_content[*index + len])); len++);
    tmp = (char *)malloc(sizeof(char) * len + 1);
    strncpy(tmp, line_content + (*index), len);
    tmp[len] = '\0';
    (*index) += len;

    if(!is_label(tmp)){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid label");
        free(tmp);
        return;
    }
    strcpy(as_tree->ast_dir_or_inst.directive.dir.label, tmp);
    free(tmp);
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] != '\n' && line_content[*index] != EOF
       && line_content[*index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after label name");
        return;
    }
}

void get_entry(char *line_content, int *index, ast *as_tree){
    char *tmp = NULL;
    int len;
    SKIP_WHITE_CHAR(line_content, *index)
	if(as_tree->label[0] != '\0'){
        as_tree->label[0] = '\0';
    }
    if(line_content[*index] == '\n' || line_content[*index] == EOF
       || line_content[*index] == '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Must be named");
        return;
    }
    for(len = 0; line_content[*index + len] != '\n' && line_content[*index + len] != EOF
                 && line_content[*index + len] != '\0' && !(isspace(line_content[*index + len])); len++);
    tmp = (char *)malloc(sizeof(char) * len + 1);
    strncpy(tmp, line_content + (*index), len);
    tmp[len] = '\0';
    (*index) += len;

    if(!is_label(tmp)){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid label");
        free(tmp);
        return;
    }
    strcpy(as_tree->ast_dir_or_inst.directive.dir.label, tmp);
    free(tmp);
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] != '\n' && line_content[*index] != EOF
       && line_content[*index] != '\0'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after label name");
        return;
    }
}


void get_dir(char *line_content, int *index, ast *as_tree){
    (*index)++;
    if(strncmp(line_content + (*index), "data", DATA_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_data_type;
        (*index) += DATA_LENGTH;
    }
    else if(strncmp(line_content + (*index), "string", STRING_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_string_type;
        (*index) += STRING_LENGTH;
    }
    else if(strncmp(line_content + (*index), "extern", EXTERN_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_extern_type;
        (*index) += EXTERN_LENGTH;
    }
    else if(strncmp(line_content + (*index), "entry", ENTRY_LENGTH) == 0){
        as_tree->ast_line_option = ast_directive;
        as_tree->ast_dir_or_inst.directive.directive_type = dir_entry_type;
        (*index) += ENTRY_LENGTH;
    }
    else{
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Dir not found");
        return;
    }
}

operand_type_num check_op(char *op, ast *as_tree){
    /* int i; */
    int val;
    char *num_end;
    char *num_val;
    /* char *index; */

    if(strlen(op) == 0){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Missing operand after op code");
        return error;
    }
    if(all_digit(op) || op[0] == '-'){
        if(all_digit(op)){
            num_val = &op[0];
        }
        else if(all_digit(++op)){
            num_val = &op[0];
        }
        val = strtol(num_val, &num_end, BASE_TEN);
        if(*num_end != '\0'){
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Not a valid number");
            return error;
        }
        if(val >= -(1 << BITS_TO_MOVE) && val < (1 << BITS_TO_MOVE)){
            return immediate;
        }
        else{
            as_tree->ast_line_option = ast_error_line;
            strcpy(as_tree->ast_error, "Number is out of bounds");
            return error;
        }
    }
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
    else if(is_label(op)){
        return label;
    }
    else{
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Not a valid operand");
        return error;
    }
}

void get_group_a_op(char *line_content, int *index, ast *as_tree){
    int len;
    char *comma;
    char *op;

    if((comma = strchr(line_content + (*index), ',')) == NULL){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Group A operands should have a comma between them");
        return;
    }
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] == ','){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Command should start with an operand, not a comma");
        return;
    }
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != EOF && line_content[*index + len] != '\n'
                 && !(isspace(line_content[*index + len])) && line_content[*index + len] != ','; len++);
    op = malloc(sizeof(char) * len + 1);
    strncpy(op, line_content + *index, len);
    op[len] = '\0';
    as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[0] = check_op(op, as_tree);
    if(as_tree->ast_line_option == ast_error_line){
        strcpy(as_tree->ast_error, "The first operand is not legal");
        free(op);
        return;
    }
    else{
        if(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[0] == immediate){
            as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_values[0].immediate = atoi(op + 1);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[0] == label){
            strcpy(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_values[0].label, op);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[0] == regstr){
            as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_values[0].reg = op[2];
        }
    }
    free(op);
    (*index) += len;
    SKIP_WHITE_CHAR(line_content, *index)
    (*index)++;
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] == ','){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excessive commas found");
        return;
    }
    for(len = 0; line_content[*index + len] != '\0' && line_content[*index + len] != EOF && line_content[*index + len] != '\n'
                 && !(isspace(line_content[*index + len])) && line_content[*index + len] != ','; len++);
    op = malloc(sizeof(char) * len + 1);
    strncpy(op, line_content + *index, len);
    op[len] = '\0';
    as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[1] = check_op(op, as_tree);
    if(as_tree->ast_line_option == ast_error_line){
        strcpy(as_tree->ast_error, "The second operand is not legal");
        free(op);
        return;
    }
    else{
        if(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[1] == immediate){
            as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_values[1].immediate = atoi(op + 1);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[1] == label){
            strcpy(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_values[1].label, op);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_types[1] == regstr){
            as_tree->ast_dir_or_inst.instruction.op_code_group.a_group_op_codes.op_values[1].reg = op[2];
        }
    }
    free(op);
    (*index) += len;
    SKIP_WHITE_CHAR(line_content, *index)
    if(line_content[*index] != '\0' && line_content[*index] != EOF && line_content[*index] != '\n'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Excess characters after second op");
        return;
    }
}

void get_group_b_op(char *line_c, int *index, ast *as_tree){
    int len;
    /* char *comma; */
    char *op;

    SKIP_WHITE_CHAR(line_c, *index)
    if(line_c[*index] == ','){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "Command should start with an operand, not a comma");
        return;
    }
    for(len = 0; line_c[*index + len] != '\0' && line_c[*index + len] != EOF && line_c[*index + len] != '\n'
                 && !(isspace(line_c[*index + len])); len++);
    op = malloc(sizeof(char) * len + 1);
    strncpy(op, line_c + *index, len);
    op[len] = '\0';
    (*index) += len;
    SKIP_WHITE_CHAR(line_c, *index)
    if(line_c[*index] != '\0' && line_c[*index] != EOF && line_c[*index] != '\n'){
        as_tree->ast_line_option = ast_error_line;
        strcpy(as_tree->ast_error, "There are extra characters after operand");
        free(op);
        return;
    }
    as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_type = check_op(op, as_tree);
    if(as_tree->ast_line_option == ast_error_line){
        free(op);
        return;
    }
    else{
        if(as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_type == immediate){
            as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_value.immediate = atoi(op);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_type == label){
            strcpy(as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_value.label, op);
        }
        else if(as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_type == regstr){
            as_tree->ast_dir_or_inst.instruction.op_code_group.b_group_op_codes.op_value.reg = op[2];
        }
        free(op);
        return;
    }
}

ast line_to_ast(char *line_c){
    int i = 0, index = 0;
    ast as_tree = {0};
    char *pointer, *tmp = NULL;
    SKIP_WHITE_CHAR(line_c, i)
    if(line_c[i] == '\0' || line_c[i] == EOF || line_c[i] == '\n'){
        as_tree.ast_line_option = ast_empty_line;
        return as_tree;
    }
    else if(line_c[i] == ';'){
        as_tree.ast_line_option = ast_comment_line;
        return as_tree;
    }
    SKIP_WHITE_CHAR(line_c, i) /* @TODO CHECK IF NEEDED */
    if((pointer = strchr(line_c, ':')) != NULL){
        index = pointer - (line_c + i);
        tmp = malloc(sizeof(char) * index + 1);
        strncpy(tmp, line_c + i, index);
        tmp[index] = '\0';
        if(is_label(tmp)){
            strcpy(as_tree.label, tmp);
            free(tmp);
            index += (i + 1);
        }
        else{
            as_tree.ast_line_option = ast_error_line;
            strcpy(as_tree.ast_error, "Label is not valid");
            free(tmp);
            return as_tree;
        }
    }
    else{
        as_tree.label[0] = '\0';
    }

    SKIP_WHITE_CHAR(line_c, index)
    if(line_c[index] == '.'){
        get_dir(line_c, &index, &as_tree);
        SKIP_WHITE_CHAR(line_c, index)
        if(as_tree.ast_line_option == ast_error_line){
            return as_tree;
        }
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_string_type){
            get_string(line_c, &index, &as_tree);
            return as_tree;
        }
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_data_type){
            get_data(line_c, &index, &as_tree);
        }
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_entry_type){
            get_entry(line_c, &index, &as_tree);
            return as_tree;
        }
        else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_extern_type){
            get_extern(line_c, &index, &as_tree);
            return as_tree;
        }
    }
    else{
        get_inst(line_c, &index, &as_tree);
        if(as_tree.ast_line_option == ast_error_line){
            return as_tree;
        }
        SKIP_WHITE_CHAR(line_c, index)
        if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_mov ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_cmp ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_add ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_sub ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_lea){
            get_group_a_op(line_c, &index, &as_tree);
            return as_tree;
        }

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

        if(as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_stop ||
           as_tree.ast_dir_or_inst.instruction.op_code.op_c == op_rts){
            SKIP_WHITE_CHAR(line_c, index)
            if(line_c[index] != '\0' && line_c[index] != EOF && line_c[index] != '\n'){
                as_tree.ast_line_option = ast_error_line;
                strcpy(as_tree.ast_error, "There can't be any operands or characters after group c op_codes");
                return as_tree;
            }
            return as_tree;
        }
    }
    return as_tree;
}

