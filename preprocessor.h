/* This file underlines the pre-processor in the assembler -
 * It goes through the file and spreads the macros defined there
 * and saves the new (without the macro definition, and with the
 * macro already separated) file - with format .am */

#ifndef FINAL_PROJECT_PREPROCESSOR_H
#define FINAL_PROJECT_PREPROCESSOR_H

#include <stdbool.h>

/* goes through the file and spreads the macros defined there
 * and saves the new (without the macro definition, and with the
 * macro already separated) file - with format .am
 * @param file_name - the file name without format
 * @return true - if there were no errors */
bool pre_process(char *file_name);

#endif
