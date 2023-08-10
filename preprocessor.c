/*
Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "global_att.h"
#include "other_functions.h"
#include "macro_table.h"
#include <string.h>

/* @TODO define numbers*/

bool pre_process(char *file_name){
    int index = 0, str_num = 0;
    bool macro_read = false;
    char curr_line[MAX_LINE_LENGTH];
    char delim[6] = " \r\n\0\t";
    char *macro_name = NULL;
    macro_table *m_table = new_macro_table();
    macro *new_m;
    char **split_string;
    char *as_file_name = str_cat(file_name, ".as");
    char *am_file_name = str_cat(file_name, ".am");
    FILE *as_file = fopen(as_file_name, "r");
    FILE *am_file;


    if(as_file == NULL){
        printf("Error opening file %s\n", as_file_name);
        free(am_file_name);
        free(as_file_name);
        fclose(as_file);
        return false;
    }

    am_file = fopen(am_file_name, "w+");
    if(am_file == NULL){
        printf("Error opening file %s\n", am_file_name);
        free(am_file_name);
        free(as_file_name);
        fclose(am_file);
        fclose(as_file);
        return false;
    }

    while(fgets(curr_line, MAX_LINE_LENGTH, as_file)){
        index = 0;
        skip_white_char(curr_line, index);
        if(is_empty(curr_line[index])){
            continue;
        }
        /*str_num = string_num(curr_line, delim);*/
        split_string = strings(curr_line, delim);
        /*if the macro is defined already, print in to the file*/
        if(get_macro_content_from_table(m_table, split_string[0]) != NULL){
            fputs(get_macro_content_from_table(m_table, split_string[0]), am_file);
        }
            /*if the macro is going to be defined here (mcr)*/
        else if(strcmp(split_string[0], "mcro") == 0){
            macro_read = true;
            macro_name = (char *) malloc(sizeof(char) * MAX_MACRO_NAME_SIZE);
            strcpy(macro_name, split_string[1]);
            new_m = new_macro(macro_name, "");
            add_macro_to_table(m_table, new_m);
        }
            /*if macro definition is ending (endmcr)*/
        else if(strcmp(split_string[0], "endmcro") == 0){
            macro_read = false;
        }
            /*if inside the macro definition*/
        else if(macro_read == true){
            char *macro_update = str_cat(get_macro_content_from_table(
                    m_table, macro_name), curr_line);
            update_macro_content(m_table, macro_name, macro_update);
        }

            /*if regular line - add line to file*/
        else{
            fputs(curr_line, am_file);
        }
        free_string_p(split_string, str_num);
    }

    /*EOF*/
    free_macro_table(m_table);
    fclose(am_file);
    fclose(as_file);
    free(am_file_name);
    free(as_file_name);

    return true;

}