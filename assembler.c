//
// Created by לידור on 11/06/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "other_functions.h"

int main(int argc, char *argv[]){
    //gets a file input and processes it
    char *string = "   Hey, My name is Lidor";
    int i;

    i = skip_white_char(string, 0);
    printf("i= %d\n", i);
    printf("string= %s\n", string[i]);

    return 0;

}
