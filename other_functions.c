//
// Created by לידור on 11/06/2023.
//

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int skip_white_char(char *string, int index){
    for(; string[index] && isspace(string[index]); index++);
    return index;
}

char *str_cat(char *str1, char *str2){
    char *final_str = (char *) malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(final_str, str1);
    strcat(final_str, str2);
    return final_str;
}