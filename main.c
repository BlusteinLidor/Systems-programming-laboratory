/*
 Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <stdio.h>
#include "global_att.h"
#include "other_functions.h"
#include "preprocessor.h"
#include "macro_table.h"
#include <stdbool.h>
#include "symbol_table.h"
#include "ast.h"
#include "first_pass.h"
#include "second_pass.h"
#include "file_types.h"

bool file_assem(char *file_name);

int main(int argc, char *argv[]){
    /*gets a file input and processes it*/

    int i = 1;
    if(argc < 2){
        fprintf(stderr, "No file was entered");
        exit(FAIL);
    }
    for(; i < argc; i++){
        if(strlen(argv[i]) > MAX_LINE_LENGTH){
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
    unsigned int ic = 0;
    unsigned int dc = 0;
    char *am_file_name = NULL;
    FILE *file_orig = NULL;
    char curr_line[MAX_LINE_LENGTH], tmp;
    symbol_table *s_table = NULL;
    ast as_tree;
    line_content line_c;
    data_m_word d_word[MEMORY_SIZE] = {0};
    code_m_word c_word[MEMORY_SIZE] = {0};
    bool entry_read = false;
    bool extern_read = false;
    bool success_read = true;


    /* preprocessor - macro separating */
    if(!pre_process(file_name)){
        printf("There was a problem separating the macros"
               " in file %s.as\n", file_name);
        return FAIL;
    }

    am_file_name = str_cat(file_name, ".am");
    file_orig = fopen(am_file_name, "r");
    if(file_orig == NULL){
        printf("Error opening file %s\n", am_file_name);
        free(am_file_name);
        return FAIL;
    }

    /* first pass */
    s_table = new_symbol_table();
    line_c.file_name = am_file_name;
    line_c.content = curr_line;
    for(line_c.line_number = 0; fgets(curr_line, MAX_LINE_LENGTH + 2,
                                      file_orig) != NULL; line_c.line_number++){
        /* check if the line exceeds the line length */
        if(strchr(curr_line, '\n') == NULL && !feof(file_orig)){
            print_error(&line_c, "Line is too long. max line size should be 80.");
            success_read = FAIL;
            do{
                tmp = fgetc(file_orig);
            } while(tmp != EOF && tmp != '\n');
        }
        else{
            as_tree = line_to_ast(curr_line);
            if(as_tree.ast_line_option == ast_error_line){
                print_error(&line_c, as_tree.ast_error);
                success_read = FAIL;
            }
            else{
                if(as_tree.ast_line_option == ast_directive){
                    if(as_tree.ast_dir_or_inst.directive.directive_type == dir_entry_type){
                        entry_read = true;
                    }
                    else if(as_tree.ast_dir_or_inst.directive.directive_type == dir_extern_type){
                        extern_read = true;
                    }
                    success_read = first_pass_process_line(&ic, &dc, line_c, s_table, d_word, c_word, &as_tree);
                }
            }
        }
        update_data_sym_address(s_table, ic);

        /* second pass */
        rewind(file_orig);
        if(success_read){
            for(line_c.line_number = 1; fgets(curr_line, MAX_LINE_LENGTH,
                                              file_orig) != NULL; line_c.line_number++){
                as_tree = line_to_ast(curr_line);
                success_read = second_pass_process_line(line_c, s_table, as_tree);
            }
            second_pass_process_label(s_table, c_word, ic);
        }
        if(ic + dc > MEMORY_SIZE - IC_INIT_VALUE){
            print_error(&line_c, "Memory size is too small");
            success_read = false;
        }
        if(success_read){
            file_type_ob(file_name, c_word, d_word, ic, dc);
            if(entry_read){
                file_type_ent(file_name, s_table);
            }
            if(extern_read){
                file_type_ext(file_name, s_table, c_word, ic);
            }
            free(am_file_name);
            fclose(file_orig);
            free_symbol_table(s_table);
            free_c_word(c_word, ic);
            return true;
        }
    }





}
