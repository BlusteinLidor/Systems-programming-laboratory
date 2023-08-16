#include "dir_process.h"

/* process a character into machine code */
data_m_word process_char(char ch){
    data_m_word d_word = {0};
    unsigned int num = (unsigned int)ch;
    d_word.data_or_string = num;
    return d_word;
}

/* convert data machine code */
data_m_word int_to_bits_dir(int num){
    data_m_word w;
    /* if the number is negative - convert it by using
     * 2's compliment */
    if(num < 0){
        unsigned int pos_val = abs(num);
        unsigned int comp_val = (~pos_val) + 1;
        w.data_or_string = (1 << 11) | (comp_val & 0xFFF);
    }
    else{
        w.data_or_string = num & 0xFFF;
    }
    return w;
}

void process_data(unsigned int *dc, data_m_word *d_word, int num_arr[], int num_count){
    int i;
    /* process each number into machine code */
    for(i = 0; i < num_count; i++){
        d_word[(*dc)++] = int_to_bits_dir(num_arr[i]);
    }
}

void process_string(unsigned int *dc, data_m_word *d_word, char string[]){
    int i;
    /* process each character into machine code */
    for(i = 0; string[i] != '\0'; i++){
        d_word[(*dc)++] = process_char(string[i]);
    }
    /* add ending of string machine code (0) */
    d_word[(*dc)++] = process_char('\0');
}
