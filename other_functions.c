//
// Created by לידור on 11/06/2023.
//

#include <stdlib.h>
#include <ctype.h>

int skip_white_char(char *string, int index){
    for(; string[index] && isspace(string[index]); index++);
    return index;
}