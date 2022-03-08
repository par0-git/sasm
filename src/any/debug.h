/**
 * SemiAssembly / sasm shared code
 * @file debug.h
 * @author lotuspar / par0-git
 * @brief Debug functions
 */

#define SASM_DEBUG
#include <stdio.h> // printf

#define __sasm_print_template "%s :: "
#define __sasm_debug_print_template "%s :: "
#define sasm_print(message, ...) printf(__sasm_print_template message "\n", __func__, ##__VA_ARGS__)
#define sasm_debug_print(message, ...) printf(__sasm_debug_print_template message "\n", __func__, ##__VA_ARGS__)

#ifndef SASM_DEBUG
    #define sasm_debug_print ;
#endif