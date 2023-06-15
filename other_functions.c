/*
 Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "globals.h"

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

bool is_empty(char c){
    if(c == '\n' || c == '\0' || c == EOF || c == ';'){
        return true;
    }
    return false;
}

int string_num(char *str, char *delim){
    int count = 0;
    char *temp;

    temp = strtok(str, delim);

    while(temp != NULL){
        count++;
        temp = strtok(NULL, delim);
    }

    return count;
}

char **strings(char *string, char *delim){
    char *token;
    char *temp = (char *) malloc(sizeof(char ) * MAX_LINE_LENGTH);
    int str_num = string_num(string, delim), i = 0;
    char **strings = malloc(sizeof(char *) * str_num);
    if(strings == NULL){
        free(strings);
        return NULL;
    }

    token = strtok(string, delim);

    while(token != NULL && i < str_num){
        strcpy(temp, token);
        /*temp = strdup(token);*/
        strings[i] = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        if(strings[i] == NULL){
            free(strings[i]);
            free(strings);
            return NULL;
        }
        strcpy(strings[i++], temp);
    }
    return strings;
}

void free_string_p(char **strings, int string_num){
    int i;
    for(i = 0; i < string_num; i++){
        free(strings[i]);
    }
    free(strings);
}
