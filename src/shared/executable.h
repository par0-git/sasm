#pragma once
#include <stdint.h>
namespace sasm {
    /**
     * sasm::executable_header
     */
    struct executable_header {
        uint8_t signature[2]; // [sa] / [0x73, 0x61] 
        uint64_t offset; // (executable_ptr + offset) == first instruction
    }

    /**
     * sasm::executable_data_type
     * Typedef for values of executable data and instructions
     */
    typedef uint8_t executable_data_type;
}