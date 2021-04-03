#pragma once
#include <stdint.h>
namespace sasm {
    namespace vm {
        /**
         * Processor Memory Layout
         * This is how memory should be used on a virtual processor

         * uint8_t[8] Processor byte flags:
           * uint8_t current variable size / word size
           * uint8_t execution status (bitflags)
            * bit execution status: 
           * ... [7]

         * register_type[register_enum::xx] Processor registers

         * uint8_t[stack_size] / register_type[?] Stack memory

         * executable_data_type[?] Executable

         * uint8_t[>=4] Post-executable padding
         */

        /**
         * sasm::vm::memory 
         * Statically sized parts of processor memory
         */
        namespace memory {
            /* "Processor byte flags" */
            struct flags {
                uint8_t word_size; // "current variable size / word size"
                uint8_t execution; // "execution status"
                uint8_t pad[6]; // "..."
            };

            /**
             * "Processor byte flags"->"execution" 
             * These can all be set at the same time!
             */
            enum execution_status {
                active = 0x01, // "active" The processor is running
                waiting = 0x02 // "waiting" The processor is running but not reading instructions
            };
        }
    }
}