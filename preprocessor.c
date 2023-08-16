#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "global_att.h"
#include "other_functions.h"
#include "macro_table.h"
#include <string.h>

bool pre_process(char *file_name){
    int index, str_num = 0;
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

    /* if couldn't open the as file print error and free all memory
     * return false */
    if(as_file == NULL){
        printf("Error opening file %s\n", as_file_name);
        free(am_file_name);
        free(as_file_name);
        fclose(as_file);
        return false;
    }

    /* open the target file to write to */
    am_file = fopen(am_file_name, "w+");
    /* if the file couldn't be opened - print error and free
     * memory allocated, return false */
    if(am_file == NULL){
        printf("Error opening file %s\n", am_file_name);
        free(am_file_name);
        free(as_file_name);
        fclose(am_file);
        fclose(as_file);
        return false;
    }
    /* if the file was opened - go through all lines with a
     * while loop */
    while(fgets(curr_line, MAX_LINE_LENGTH, as_file)){
        index = 0;
        /* skip all white characters */
        skip_white_char(curr_line, index);
        /* if the line is empty (only white characters) - continue
         * to the next line */
        if(is_empty(curr_line[index])){
            continue;
        }
        /* split the lines into an array of strings by the
         * delimiters */
        split_string = strings(curr_line, delim);
        /*if the macro is defined already, write it to the file*/
        if(get_macro_content_from_table(m_table, split_string[0]) != NULL){
            fputs(get_macro_content_from_table(m_table, split_string[0]), am_file);
        }
        /*if the macro is going to be defined here */
        else if(strcmp(split_string[0], "mcro") == 0){
            /* mark begging of macro definition */
            macro_read = true;
            /* allocate memory for macro name */
            macro_name = (char *) malloc(sizeof(char) * MAX_MACRO_NAME_SIZE);
            /* assign the macro name to macro_name */
            strcpy(macro_name, split_string[1]);
            /* make a new macro without content (will be read by next lines) */
            new_m = new_macro(macro_name, "");
            /* add the macro to the macro table */
            add_macro_to_table(m_table, new_m);
        }
        /*if macro definition is ending */
        else if(strcmp(split_string[0], "endmcro") == 0){
            /* mark out of macro definition */
            macro_read = false;
        }
        /*if inside the macro definition*/
        else if(macro_read == true){
            /* concatenate between the last macro definition
             * and the current line macro definition*/
            char *macro_update = str_cat(get_macro_content_from_table(
                    m_table, macro_name), curr_line);
            /* update macro content */
            update_macro_content(m_table, macro_name, macro_update);
        }
        /*if regular line - add line to file*/
        else{
            fputs(curr_line, am_file);
        }
        /* free allocated memory */
        free_string_p(split_string, str_num);
    }

    /*EOF - free memory and close files */
    free_macro_table(m_table);
    fclose(am_file);
    fclose(as_file);
    free(am_file_name);
    free(as_file_name);

    return true;

}
