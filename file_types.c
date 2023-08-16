#include "file_types.h"


void file_type_ob(char *file_name, code_m_word code_m[], data_m_word data_m[],
                  unsigned int ic, unsigned int dc){
    /* .ob file */
    FILE *ob_file;
    int i;
    int val1, val2;
    /* make the .ob format file name */
    char *ob_file_name = str_cat(file_name, ".ob");
    /* open the ob file for writing */
    ob_file = fopen(ob_file_name, "w");
    /* if couldn't open - print error, free memory and return*/
    if(ob_file == NULL){
        printf("Error opening file %s\n", ob_file_name);
        free(ob_file_name);
        return;
    }
    /* print the header of the file - number of ic and dc */
    fprintf(ob_file, "\t\t%d %d\n", ic, dc);
    /* go through all machine code words and divide each
     * to 2 parts - 6 MSB and 6 LSB - and turn them into
     * base 64 and print them */
    for(i = 0; i < ic; i++){
        if(code_m[i].c_word.im_dir != NULL){
            val1 = *(int *)code_m[i].c_word.im_dir;
			val2 = (val1 >> 6);
        }
        fprintf(ob_file, "%d\t\t", i + IC_INIT_VALUE);
		fprintf(ob_file, "%c", num_to_base_64(val2));
		fprintf(ob_file, "%c", num_to_base_64(val1));
        fprintf(ob_file, "\n");
    }
    /* go through all data words and divide each
     * to 2 parts - 6 MSB and 6 LSB - and turn them into
     * base 64 and print them */
	for(i = 0; i < dc; i++){
		val1 = data_m[i].data_or_string;
		val2 = (val1 >> 6);
        fprintf(ob_file, "%d\t\t", i + ic + IC_INIT_VALUE);
        fprintf(ob_file, "%c", num_to_base_64(val2));
		fprintf(ob_file, "%c", num_to_base_64(val1));
        fprintf(ob_file, "\n");
    }
    /* free memory and close files */
    free(ob_file_name);
    fclose(ob_file);
}

void file_type_ext(char *file_name, symbol_table *s_table, code_m_word code_m[],
                   data_m_word data_m[], unsigned int ic){
    /* .ext file */
    FILE *ext_file;
    int count = 0;
    int i;
    /* make the .ext format file name */
    char *ext_file_name = str_cat(file_name, ".ext");
    /* check how many labels there are */
    for(i = 0; i < ic; i++){
        if(code_m[i].label != NULL){
            count++;
        }
    }
    /* no labels - free memory and return */
    if(count == 0){
        free(ext_file_name);
        return;
    }
    /* open ext file for writing */
    ext_file = fopen(ext_file_name, "w");
    /* if couldn't open - print error, free memory and return*/
    if(ext_file == NULL){
        printf("Error opening file %s\n", ext_file_name);
        free(ext_file_name);
        return;
    }
    /* for every label that is of extern type - print the label
     * name and the address in which it can be found */
    for(i = 0; i < ic; i++){
        if(code_m[i].label != NULL){
            symbol_type sym_type = get_symbol_type_from_table(s_table, code_m[i].label);
            if(sym_type == extern_symbol){
                fprintf(ext_file, "%s   %d\n", code_m[i].label, i + IC_INIT_VALUE);
            }
        }
    }
    /* free memory and close files */
    free(ext_file_name);
    fclose(ext_file);
}

void file_type_ent(char *file_name, symbol_table *s_table){
    /* .ent file */
    FILE *ent_file;
    int i;
    /* make the .ent format file name */
    char *ent_file_name = str_cat(file_name, ".ent");
    /* open ent file for writing */
    ent_file = fopen(ent_file_name, "w");
    /* if couldn't open - print error, free memory and return*/
    if(ent_file == NULL){
        printf("Error opening file %s\n", ent_file_name);
        free(ent_file_name);
        return;
    }
    /* for every symbol that is of entry type - print the label
     * name and the address in which it can be found */
    for(i = 0; i < TABLE_SIZE; i++){
        symbol *sym = s_table->symbols[i];
        if(sym == NULL){
            continue;
        }
        if(sym->symbol_t == entry_symbol){
            fprintf(ent_file, "%s\t\t%d\n", sym->symbol_name, (sym->symbol_address) + IC_INIT_VALUE);
        }
    }
    /* free memory and close files */
    free(ent_file_name);
    fclose(ent_file);
}
