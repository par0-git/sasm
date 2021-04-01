#pragma once

#include <stdio.h> // printf
#define sasm_print_template "%s :: "
#define sasm_print(message, ...) printf(sasm_print_template message "\n", __func__, ##__VA_ARGS__)
