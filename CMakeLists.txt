cmake_minimum_required(VERSION 3.25)
project(final_project C)

set(CMAKE_C_STANDARD 90)

add_executable(final_project global_att.h main.c preprocessor.c first_pass.c second_pass.c macro_table.c ast.c other_functions.c other_functions.h macro_table.h preprocessor.h symbol_table.c symbol_table.h ast.h second_pass.h file_types.c file_types.h first_pass.h inst_process.c inst_process.h dir_process.c dir_process.h)
