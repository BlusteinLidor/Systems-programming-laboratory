cmake_minimum_required(VERSION 3.25)
project(final_project C)

set(CMAKE_C_STANDARD 90)

add_executable(final_project globals.h assembler.c preprocessor.c first_pass.c second_pass.c tables.c ast.c output_files.c other_functions.c other_functions.h tables.h)
