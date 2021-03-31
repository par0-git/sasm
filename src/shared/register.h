#pragma once
#include <stdint.h> // uint64_t

namespace sasm {
    /**
     * sasm::register_enum
     * Registers used in the virtual processor / interpreter. Called register_enum due to C/C++ having the "register" storage class
     * [[This is not the final list of registers!]] SASM is in development and these might change.
     */
    enum register_enum {
        r0,
        r1,
        r2,
        r3,
        r4,
        r5,
        r6,
        r7,
        ri, // Instruction register: pointer to current instruction
        rl, // Size / length register: size of variables
        rs, // Stack register: pointer to current position in stack
        xx  // ((end of enum))
    };

    /**
     * sasm::register_type
     * Typedef for values of register data
     */
    typedef uint64_t register_type;
}