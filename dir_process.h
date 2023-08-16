/* This file underlines the processing of directive lines
 * into machine code */

#ifndef FINAL_PROJECT_DIR_PROCESS_H
#define FINAL_PROJECT_DIR_PROCESS_H

#include <stdlib.h>
#include "global_att.h"

/* process the data line into machine code
 * @param dc - data count
 * @param d_word - data word
 * @param num_arr[] - number array
 * @param num_count - number array length */
void process_data(unsigned int *dc, data_m_word *d_word, int num_arr[], int num_count);

/* process string into machine code
 * @param dc - data count
 * @param d_word - data word
 * @param string[] - characters to process*/
void process_string(unsigned int *dc, data_m_word *d_word, char string[]);

#endif
