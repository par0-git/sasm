#pragma once
namespace sasm {
    /**
    * sasm::opcode
    * Opcodes for instructions read by the virtual processor
    * [[This is not the final list of opcodes!]] SASM is in development and these might change.
    */
    enum opcode {
        /** WordSize
         *  Set working size {argument 1} of variables (register rl)
         */
        ws = 0x00,
        /** Push
         *  Push register {argument 1} to the stack
         */
        push = 0x10,
        /** Pop
         *  Pop register {argument 1} from the stack
         */
        pop = 0x20,
        /** Mov
         *  Move variable {argument 2} to register {argument 1}
         */
        mov = 0x30,
        /** Inc
         *  Increment value of register {argument 1}
         */
        inc = 0x40,
        /** Dec
         *  Decrement value of register {argument 1}
         */
        dec = 0x50,
        /** Print
         *  Print value of register {argument 1}
         */
        print = 0x60
    };
}