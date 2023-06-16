/*
 Created by לידור on 11/06/2023.
*/

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "other_functions.h"
#include "preprocessor.h"
#include "macro_table.h"
#include <stdbool.h>

int main(int argc, char *argv[]){
    /*gets a file input and processes it*/

    /*this is a try for the preprocessor*/
    if(argc == 2){
        pre_process(argv[1]);
    }

    return 0;

}
