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
    int ic = 0;
    int dc = 0;
    char *am_file_name = NULL;
    FILE *am_file = NULL;
    char curr_line[MAX_LINE_LENGTH];
    symbol_table *s_table = NULL;
    ast as_tree;
    line_content line_c;
    im_or_dir_m_word im_dir[MEMORY_SIZE] = {0};
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
    am_file = fopen(am_file_name, "r");
    if(am_file == NULL){
        printf("Error opening file %s\n", am_file_name);
        free(am_file_name);
        return FAIL;
    }

    /* first pass */
    s_table = new_symbol_table();
    line_c.file_name = am_file_name;
    line_c.content = curr_line;
    for(line_c.line_number = 0; fgets(curr_line, MAX_LINE_LENGTH,
                                      am_file) != NULL; line_c.line_number++){
        /* check if the line exceeds the line length */
        if(strchr(curr_line, '\n') == NULL && !feof(am_file)){
            /* @TODO print error message - line is too long */
            success_read = FAIL;
        }
        else{
            /* as_tree = @TODO add to ast.c a function which
             * @TODO makes an ast from the current line*/
            if(as_tree.ast_line_option == ast_error_line){
                /* @TODO print error message - ast error */
                success_read = FAIL;
            }
            else{
                if(as_tree.ast_line_option == ast_directive){

                }
            }
        }
    }





}
