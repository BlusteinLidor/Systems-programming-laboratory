#include "global_att.h"
#include "other_functions.h"
#include "symbol_table.h"
#include "ast.h"
#include "inst_process.h"
#include "dir_process.h"

bool first_pass_process_line(unsigned int *ic, unsigned int *dc, line_content line_c,
                        symbol_table *s_table, data_m_word *d_word,
                        code_m_word *c_word, ast *as_tree){
    symbol *sym = NULL;
    /* if a label was detected */
    if(as_tree->label[0] != '\0'){
        /* if the label is already in the symbol table */
        if(get_symbol_from_table(s_table, as_tree->label) != NULL){
            /* if it's an extern symbol type print error and return false */
            if(get_symbol_type_from_table(s_table, as_tree->label) == extern_symbol){
                print_error(&line_c, "The extern label is trying to be defined in the same file");
                return false;
            }
            else{
                print_error(&line_c, "The label is already defined");
                return false;
            }
        }
        /*if it's a directive line */
        if(as_tree->ast_line_option == ast_directive){
            /* if it's a .data or .string data */
            if(as_tree->ast_dir_or_inst.directive.directive_type == dir_data_type
            || as_tree->ast_dir_or_inst.directive.directive_type == dir_string_type){
                /* if a label was detected and is not in the symbol table already */
                if(as_tree->label[0] != '\0' && get_symbol_from_table(s_table, as_tree->label) == NULL){
                    /* add it to the symbol table */
                    sym = new_symbol(as_tree->label, *dc, data_symbol);
                    add_symbol_to_table(s_table, sym);
                }
                /* if it's a .string - process it into machine code */
                if(as_tree->ast_dir_or_inst.directive.directive_type == dir_string_type){
                    process_string(dc, d_word, as_tree->ast_dir_or_inst.directive.dir.str);
                }
                    /* if it's a .data - process it into machine code */
                else{
                    process_data(dc, d_word, as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr,
                                 as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr_size);
                }
            }
            /* if it's extern or entry - print error and return false */
            else if(as_tree->ast_dir_or_inst.directive.directive_type == dir_entry_type ||
            as_tree->ast_dir_or_inst.directive.directive_type == dir_extern_type){
                print_error(&line_c, "Entry/extern aren't considered in label definition");
                return false;
            }
        }
		/* if it's an instruction line */
        if(as_tree->ast_line_option == ast_instruction){
            /* add it to the symbol table and return true if process inst succeeded  */
            sym = new_symbol(as_tree->label, *ic, code_symbol);
            add_symbol_to_table(s_table, sym);
            return process_inst(&line_c, ic, s_table, c_word, as_tree);
        }
    }
    /* if it's not a label declaration */
    else{
        /*if it's a directive line */
        if(as_tree->ast_line_option == ast_directive){
            if(as_tree->ast_dir_or_inst.directive.directive_type == dir_data_type ||
            as_tree->ast_dir_or_inst.directive.directive_type == dir_string_type){
                if(as_tree->ast_dir_or_inst.directive.directive_type == dir_string_type){
                    process_string(dc, d_word, as_tree->ast_dir_or_inst.directive.dir.str);
                }
                else{
                    process_data(dc, d_word, as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr,
                                 as_tree->ast_dir_or_inst.directive.dir.num_arr.int_arr_size);
                }
            }
            else if(as_tree->ast_dir_or_inst.directive.directive_type == dir_entry_type){
                return true;
            }
            else if(as_tree->ast_dir_or_inst.directive.directive_type == dir_extern_type){
                sym = new_symbol(as_tree->ast_dir_or_inst.directive.dir.label, 0, extern_symbol);
                add_symbol_to_table(s_table, sym);
                return true;
            }
        }
        /* if it's an instruction line */
        else if(as_tree->ast_line_option == ast_instruction){
            return process_inst(&line_c, ic, s_table, c_word, as_tree);
        }
    }
    return true;
}
