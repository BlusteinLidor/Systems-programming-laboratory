//
// Created by לידור on 11/06/2023.
//

#include "globals.h"
#include "ast.h"
#include "other_functions.h"
#include "symbol_table.h"

bool second_pass_process(ast *as_tree, symbol_type *s_table, line_content line_c){
    if(as_tree->ast_line_option == ast_instruction){
        if(as_tree->ast_dir_or_inst.instruction.ins)
    }
}