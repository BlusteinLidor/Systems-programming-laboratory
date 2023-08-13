/*
 * Maman 14 (Final Project)
 * Name: Lidor Blustein
 * ID: 314993460
 *
 This project represents an assembler that translates
 human-readable assembly language code into machine code
 that a computer's central processing unit (CPU) can execute.
 It mimics the first stage of the typical compilation process,
 handling lexical analysis, parsing, and code generation.

 This is the main file for the project.
*/

#include <stdlib.h>
#include <stdio.h>
#include "global_att.h"
#include "other_functions.h"
#include "preprocessor.h"
#include <stdbool.h>
#include "symbol_table.h"
#include "ast.h"
#include "first_pass.h"
#include "second_pass.h"
#include "file_types.h"

/* This function goes through a file, reads each line,
 * saving the instructions and directives in the data
 * code table and the machine code table.
 * If there are any macro declarations in the file,
 * the program separates it anywhere mentioned in the file,
 * and makes a new file (whether there were macro
 * declarations) with the format .am (after macro).
 * Each line is then interpreted into a machine code,
 * and saved in a file format .ob (object).
 * Additionally, if there is an entry or extern directive,
 * an appropriate additional file is added, with the format
 * .ent (entry), or .ext (extern), which states the machine
 * code lines the entry/extern directives are mentioned.
 * */
bool file_assem(char *file_name);


/* The main function gets from the command line a minimum
 * of 1 .as (assembly) file name, and taking it through
 * the file_assem function (each file).
 * */
int main(int argc, char *argv[]){

    /* argc counter - if there is no files mentioned after
     * the call to the main program, the program gives an
     * error.
     * */
    int i = 1;
    if(argc < 2){
        fprintf(stderr, "No file was entered");
        exit(FAIL);
    }
    /* loops through the files */
    for(; i < argc; i++){
        if(strlen(argv[i]) > MAX_LINE_LENGTH){
            /* if the file's name is too long - print an error
             * and continue to the next file
             * */
            fprintf(stderr, "The file's '%s' name is"
                            "too long, please use less than %d"
                            "characters\n", argv[i], MAX_LINE_LENGTH-1);
            continue;
        }
        file_assem(argv[i]);

    }

    return 0;

}

bool file_assem(char *file_name){
    /* instruction counter */
    unsigned int ic = 0;
    /* data counter */
    unsigned int dc = 0;
    char *am_file_name = NULL;
    /* the current file to interpret */
    FILE *file_orig = NULL;
    /* the current line in the file*/
    char curr_line[MAX_LINE_LENGTH], tmp;
    symbol_table *s_table = NULL;
    /* Abstract syntax tree for each line */
    ast as_tree;
    line_content line_c;
    /* data words table */
    data_m_word d_word[MEMORY_SIZE] = {0};
    /* machine code words table */
    code_m_word c_word[MEMORY_SIZE] = {0};
    bool entry_read = false;
    bool extern_read = false;
    bool success_read = true;
	bool error_found = false;


    /* preprocessor - macro separating */

    /* If there was an error in the file - print error */
    if(!pre_process(file_name)){
        printf("There was a problem separating the macros"
               " in file %s.as\n", file_name);
        return FAIL;
    }

    /* concatenate the .am format to the file name */
    am_file_name = str_cat(file_name, ".am");
    /* open the file in 'read' mode */
    file_orig = fopen(am_file_name, "r");
    /* if there was an error opening the file - print error */
    if(file_orig == NULL){
        printf("Error opening file %s\n", am_file_name);
        free(am_file_name);
        return FAIL;
    }

    /* first pass */
    /* make a new symbol table */
    s_table = new_symbol_table();
    /* store file name in the ast */
    line_c.file_name = am_file_name;
    /* store current line content in the ast */
    line_c.content = curr_line;
    /* loop through each line in the .am file */
    for(line_c.line_number = 1; fgets(curr_line, MAX_LINE_LENGTH + 2,
                                      file_orig) != NULL; line_c.line_number++){
        /* check if the line exceeds the line length - if it does -
         * print error and proceed until end of line/file */
        if(strchr(curr_line, '\n') == NULL && !feof(file_orig)){
            print_error(&line_c, "Line is too long. max line size should be 80.");
            success_read = FAIL;
			error_found = SUCCESS;
            do{
                tmp = fgetc(file_orig);
            } while(tmp != EOF && tmp != '\n');
        }
        /* else, store the line in the ast */
        else{
            as_tree = line_to_ast(curr_line);
            /* if there was an error in the line - print error */
            if(as_tree.ast_line_option == ast_error_line){
                print_error(&line_c, as_tree.ast_error);
                success_read = FAIL;
				error_found = SUCCESS;
            }
            else{
                /* else, if the line is a directive, check if it's an
                 * entry or extern directive and mark it if it is*/
                if(as_tree.ast_line_option == ast_directive){
                    if(as_tree.ast_dir_or_inst.directive.directive_type == dir_entry_type){
                        entry_read = true;
                    }
                    else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_extern_type){
                        extern_read = true;
                    }
                }
                /* go through the first pass process for the line */
                success_read = first_pass_process_line(&ic, &dc, line_c, s_table, d_word, c_word, &as_tree);
            }
        }
    }
    /* update the symbol address table */
    update_data_sym_address(s_table, ic);
    /* second pass */
    /* set the pointer inside the file to the beginning of the file */
    rewind(file_orig);
    /* if there wasn't an error in the first pass - go on to second pass*/
    if(!error_found){
        /* loop through the lines */
        for(line_c.line_number = 1; fgets(curr_line, MAX_LINE_LENGTH,
                                          file_orig) != NULL; line_c.line_number++){
            /* store the line in the ast */
            as_tree = line_to_ast(curr_line);
            /* go through the second pass process for the line */
            success_read = second_pass_process_line(line_c, s_table, as_tree);
			/* if the second pass wasn't successful - mark error found*/
            if(!success_read){
               error_found = SUCCESS;
            }
        }
        /* upon finishing the second pass process for all lines -
         * process each label to machine code */
        second_pass_process_label(s_table, c_word, ic);
    }
    /* if the instruction and data count is over the memory size,
     * print error */
    if(ic + dc > MEMORY_SIZE - IC_INIT_VALUE){
        print_error(&line_c, "Memory size is too small");
        success_read = false;
		error_found = SUCCESS;
    }
    /* if there wasn't and error - make .ob file and
     * .ent and .ext files if needed */
    if(!error_found){
        file_type_ob(file_name, c_word, d_word, ic, dc);
        if(entry_read){
            file_type_ent(file_name, s_table);
        }
        if(extern_read){
            file_type_ext(file_name, s_table, c_word, d_word, ic);
        }
    }
    /* free memory and close file */
    free(am_file_name);
    fclose(file_orig);
    free_symbol_table(s_table);
    return true;
}
