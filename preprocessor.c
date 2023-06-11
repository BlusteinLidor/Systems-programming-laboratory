//
// Created by לידור on 11/06/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "globals.h"
#include "other_functions.h"
#include <string.h>

bool pre_process(char *file_name){
    int index = 0;
    char curr_line[MAX_LINE_LENGTH];
    char *macro_name = NULL;
    char *as_file_name = str_cat(file_name, ".as");
    char *am_file_name = str_cat(file_name, ".am");
    FILE *as_file = fopen(as_file_name, "r");
    FILE *am_file;
    //@TODO add macro table reference
}
//1. read next line - IF EOF - go to 9
//2. IF first word is macro from macro table - switch macro
//   name to macro content, and go to 1. ELSE proceed.
//3. IF first word is "mcro" - proceed, ELSE - go to 6
//4. turn on flag is_macro
//5. enter the macro name to the macro table
//6. read next line - if EOF - go to 9, ELSE - if is_macro
//   is TRUE and "endmcro" wasn't detected - enter the line to
//   the macro table and remove it from the file. ELSE - go
//   to 1.
//7. IF "endmcro" detected - remove it from the file and proceed
//   ELSE - go to 6.
//8. turn is_macro flag OFF (FALSE). go to 1.
//9. END: save file .am
