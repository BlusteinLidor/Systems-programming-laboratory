cmake_minimum_required(VERSION 3.25)
project(final_project C)

set(CMAKE_C_STANDARD 90)

add_executable(final_project globals.h main.c preprocessor.c first_pass.c second_pass.c macro_table.c ast.c output_files.c other_functions.c other_functions.h macro_table.h preprocessor.h symbol_table.c symbol_table.h ast.h second_pass.h)
