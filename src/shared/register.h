#pragma once
#include "number.h" // u64

namespace sasm {
    /**
     * sasm::registers
     * Registers used in the virtual processor / interpreter.
     * [[This is not the final list of registers!]] SASM is in development and these might change.
     */
    enum registers {
        r0, // Generic
        r1, // Generic
        r2, // Generic
        r3, // Generic
        rr,  // Result register: the result of comparisons etc... can be used normally
        rxi,  // Instruction register: pointer to current instruction
        rxe,  // Executable register: pointer to executable start
        rxs,  // Stack register: pointer to current position in stack
        r8, // Generic
        r9, // Generic
        r10, // Generic
        r11, // Generic
        r12, // Generic
        r13, // Generic
        r14, // Generic
        r15, // Generic
        xx  // ((end of enum))
    };
}