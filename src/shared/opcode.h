#pragma once
namespace sasm {
    /**
     * sasm::opcodes
     * Opcodes for instructions read by the virtual processor
     * [[This is not the final list of opcodes!]] SASM is in development and these might change.
     */
    enum opcodes {
        /** Extra / NOP: 0x00
         *  Nothing!
         */
        /** WordSize [5]
         *  ws: Set working size to arbitrary size (ARGUMENT 1: byte),
         *  wsa: Set working size to single byte
         *  wsb: Set working size to double byte / half word
         *  wsc: Set working size to 4 bytes / word
         *  wsd: Set working size to 8 bytes / giant word
         */
            ws = 0x01, wsa = 0x02, wsb = 0x03, wsc = 0x04, wsd = 0x05,
        /** Math [4]
         *  madd: Add number (ARGUMENT 2: working number) to register (ARGUMENT 1: register)
         *  msub: Subtract number (ARGUMENT 2: working number) from register (ARGUMENT 1: register)
         *  mmul: Multiply number stored in register (ARGUMENT 1: register) by number (ARGUMENT 2: working number) 
         *  mdiv: Divide number stored in register (ARGUMENT 1: register) by number (ARGUMENT 2: working number) 
         */
            madd = 0x06, msub = 0x07, mmul = 0x08, mdiv = 0x09, 
        /** Push [0x1*]
         *  Push register (NIBBLE: register) to the stack
         */
            push = 0x10,
        /** Pop [0x2*]
         *  Pop register (NIBBLE: register) from the stack
         */
            pop = 0x20,
        /** Set [0x3*]
         *  Move data in argument (ARGUMENT 1: working data) to register (NIBBLE: register)
         */
            set = 0x30,
        /** Move [0x4*]
         *  Move data in register (ARGUMENT 1: register) to register (NIBBLE: register)
         */
            mov = 0x40,
        /** Increment [0x5*]
         *  Increment value of register {argument 0}
         */
            inc = 0x50,
        /** Decrement [0x6*]
         *  Decrement value of register {argument 0}
         */
            dec = 0x60,    
        /** Comparisons [5]
         *  cmp*: All below comparisons set result register {rr} to 1 on success
         *  cmpr: Compare register (ARGUMENT 1: register) data with data from another register (ARGUMENT 2: register)
         *  cmpv: Compare value (ARGUMENT 1: working number) with another value (ARGUMENT 2: working number)
         *  cmprv: Compare register (ARGUMENT 1: register) to value (ARGUMENT 2: register)
         *
         *  cjmp* "Jump If True Comparison": All below comparisons jump to provided location on success (ARGUMENT 3?: pointer) 
         *  cjmpr: Compare register (ARGUMENT 1: register) data with data from another register (ARGUMENT 2: register)
         *  cjmpv: Compare value (ARGUMENT 1: working number) with another value (ARGUMENT 2: working number)
         *  cjmprv: Compare register (ARGUMENT 1: register) to value (ARGUMENT 2: register)
         *  
         *  cnjmp* "Jump If False Comparison": All below comparisons jump to provided location on non-success (ARGUMENT 3?: pointer) 
         *  cnjmpr: Compare register (ARGUMENT 1: register) data with data from another register (ARGUMENT 2: register)
         *  cnjmpv: Compare value (ARGUMENT 1: working number) with another value (ARGUMENT 2: working number)
         *  cnjmprv: Compare register (ARGUMENT 1: register) to value (ARGUMENT 2: register)
         */
            cmpr = 0x70, cmpv = 0x71, cmprv = 0x72, // [0x70-0x72]
            cjmpr = 0x73, cjmpv = 0x74, cjmprv = 0x75, // [0x73-0x75]
            cnjmpr = 0x76, cnjmpv = 0x77, cnjmprv = 0x78, // [0x76-0x78]

        xx_extra = 0xfe,
        xx_reserved = 0xff // Could be used in future as a prefix for extra opcodes
    };
}