//
// Created by לידור on 11/06/2023.
//

#include "globals.h"
#include "other_functions.h"
#include "symbol_table.h"
#include "ast.h"

bool first_pass_process(unsigned int *ic, unsigned int *dc, line_content line_c,
                        symbol_table *s_table, data_m_word *d_word,
                        code_m_word *c_word, ast *as_tree){
    symbol *sym = NULL;
    if(as_tree->label[0] != '\0'){
        if(get_symbol_address_from_table(s_table, as_tree->label) != -1){
            if(get_symbol_type_from_table(s_table, as_tree->label) == extern_symbol){
                /* @TODO print error - extern label is trying to be defined in the same file */
                return false;
            }
            else{
                /* @TODO print error - the label is already defined */
                return false;
            }
        }
        //directive
        if(as_tree->ast_line_option == ast_directive){
            if(as_tree->ast_dir_or_inst.directive.directive_type == dir_data_type
            || as_tree->ast_dir_or_inst.directive.directive_type == dir_string_type){
                if(as_tree->label && (get_symbol_address_from_table(s_table, as_tree->label) == -1)){
                    sym = new_symbol(as_tree->label, *dc, data_symbol);
                    add_symbol_to_table(s_table, sym);
                }
                if(as_tree->ast_dir_or_inst.directive.directive_type == dir_string_type){
                    /* @TODO add encode string function */
                }
            }
        }
    }
}