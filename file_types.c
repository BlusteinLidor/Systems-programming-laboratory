

#include "file_types.h"


void file_type_ob(char *file_name, code_m_word code_m[], data_m_word data_m[],
                  unsigned int ic, unsigned int dc){
    FILE *ob_file;
    int i, j;
    unsigned int val;
    char *ob_file_name = str_cat(file_name, ".ob");

    ob_file = fopen(ob_file_name, "w");
    if(ob_file == NULL){
        printf("Error opening file %s\n", ob_file_name);
        free(ob_file_name);
        return;
    }
    fprintf(ob_file, "\t\t%d %d\n", ic, dc);
    for(i = 0; i < ic; i++){
        if(code_m[i].c_word.im_dir != NULL){
            val = *(unsigned int *)code_m[i].c_word.im_dir;
        }
        fprintf(ob_file, "%d\t\t", i + IC_INIT_VALUE);
        for(j = 2; j > 0; j--){
            fprintf(ob_file, "%c", num_to_base_64(val));
            val = (val >> 6);
        }
        fprintf(ob_file, "\n");
    }
    free(ob_file_name);
    fclose(ob_file);
}

void file_type_ext(char *file_name, symbol_table *s_table, code_m_word code_m[],
                   data_m_word data_m[], unsigned int ic){
    FILE *ext_file;
    int count = 0;
    int i;
    char *ext_file_name = str_cat(file_name, ".ext");

    for(i = 0; i < ic; i++){
        if(code_m[i].label != NULL){
            count++;
        }
    }
    if(count == 0){
        free(ext_file_name);
        return;
    }
    ext_file = fopen(ext_file_name, "w");
    if(ext_file == NULL){
        printf("Error opening file %s\n", ext_file_name);
        free(ext_file_name);
        return;
    }
    for(i = 0; i < ic; i++){
        if(code_m[i].label != NULL){
            symbol_type sym_type = get_symbol_type_from_table(s_table, code_m[i].label);
            if(sym_type == extern_symbol){
                fprintf(ext_file, "%s   %d\n", code_m[i].label, i + IC_INIT_VALUE);
            }
        }
    }
    free(ext_file_name);
    fclose(ext_file);
}

void file_type_ent(char *file_name, symbol_table *s_table){
    FILE *ent_file;
    int i;
    char *ent_file_name = str_cat(file_name, ".ent");

    ent_file = fopen(ent_file_name, "w");
    if(ent_file == NULL){
        printf("Error opening file %s\n", ent_file_name);
        free(ent_file_name);
        return;
    }
    for(i = 0; i < TABLE_SIZE; i++){
        symbol *sym = s_table->symbols[i];
        if(sym == NULL){
            continue;
        }
        if(sym->symbol_t == entry_symbol){
            fprintf(ent_file, "%s\t\t%d\n", sym->symbol_name, (sym->symbol_address) + 100);
        }
    }
    free(ent_file_name);
    fclose(ent_file);
}