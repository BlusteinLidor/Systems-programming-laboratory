//
// Created by לידור on 11/06/2023.
//

#ifndef FINAL_PROJECT_OTHER_FUNCTIONS_H
#define FINAL_PROJECT_OTHER_FUNCTIONS_H
#include <stdbool.h>

int skip_white_char(char *string, int index);
char *str_cat(char *first, char *second);
bool is_empty(char c);
char **strings(char *string, char *delim);
int string_num(char *str, char *delim);
void free_string_p(char **strings, int string_num);

#endif //FINAL_PROJECT_OTHER_FUNCTIONS_H
