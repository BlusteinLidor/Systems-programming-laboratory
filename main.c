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

    /*this is a try for the preprocessor - can be removed*/
    if(argc == 2){
        pre_process(argv[1]);
    }

    /* @TODO first_pass - initialize IC and DC to 0*/
    /* @TODO first_pass - read next line*/
    return 0;

}
