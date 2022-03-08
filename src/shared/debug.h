#pragma once

#include <stdio.h> // printf
#define sasm_print_template "%s :: "
#define sasm_print(message, ...) printf(sasm_print_template message "\n", __func__, ##__VA_ARGS__)

#define SASM_DEBUG

#ifndef SASM_DEBUG
    #define sasm_debug_print ;
#else
    #define sasm_debug_print sasm_print
#endif