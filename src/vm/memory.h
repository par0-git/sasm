#pragma once
namespace sasm {
    namespace vm {
        /**
         * Processor Memory Layout
         * This is how memory should be used on a virtual processor

         * uint8_t[8] Processor byte flags:
           * uint8_t current variable size / word size
           * ... [7]

         * uint8_t[stack_size] / register_type[?] Stack memory

         * executable_data_type[?] Executable

         * uint8_t[?] Post-executable padding
         */

        /**
         * sasm::vm::memory 
         * Statically sized parts of processor memory
         */
        namespace memory {
            /* "Processor byte flags" */
            struct flags {
                uint8_t word_size; // "current variable size / word size"
                uint8_t pad[7]; // "..."
            }
        }
    }
}