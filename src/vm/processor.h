#pragma once
#include "memory.h"
#include "debug.h" // sasm_print
#include "../shared/register.h" // register_type
#include "../shared/executable.h" // executable_header
#include <stdlib.h> // malloc
#include <stdexcept> // std::invalid_argument
#include <vector> // std::vector
namespace sasm {
    namespace vm {
        /**
         * sasm::processor
         * The virtual processor. Processes instructions and executes code accordingly
         * [[This might not be final!]] SASM is in development and this might change.
         */
        class processor {
        public:
            processor(int executable_size, int stack_size) {
                int size = sizeof(memory::flags) + // Size of processor flags
                            (sizeof(register_type) * register_enum::xx) + // Space for processor registers
                            stack_size + // Space for stack
                            executable_size + // Space for executable
                            4; // 4 byte padding
                            
                sasm_print("Attempting to allocate %i bytes for virtual processor memory", size);

                // Make sure stack can hold at least 1 register
                if (sizeof(register_type) > stack_size) {
                    sasm_print("Stack size (%i) is not large enough (%lu) for a single register, aborting.", stack_size, sizeof(register_type));
                    throw std::invalid_argument("Processor memory error");
                }

                // Make sure executable space can hold executable header
                if (sizeof(executable_header) > executable_size) {
                    sasm_print("Executable space (%lu) is not large enough for a header, aborting.", sizeof(executable_header));
                    throw std::invalid_argument("Processor memory error");
                }

                // Attempt to allocate memory
                memory = (uint8_t*) malloc(size);

                // Check for success
                if (memory == 0x0) {
                    sasm_print("Failed to allocate virtual processor memory.");
                    throw std::invalid_argument("Processor memory error");
                }

                // Print success
                sasm_print("Successfully allocated virtual processor memory. PTR (%p)", memory);

                // Set pointers
                sasm_print("Setting all processor pointers...");
                ptr_flags = (memory::flags*) memory;
                ptr_registers = (register_type*)(ptr_flags + sizeof(memory::flags));
                ptr_stack = (uint64_t*)(ptr_registers + (sizeof(register_type) * register_enum::xx));
                ptr_executable = (executable_data_type*)(ptr_stack + stack_size);

                // Print pointers
                sasm_print(
                    "Set processor pointers. FLAGS (%p), REGISTERS (%p), STACK (%p), EXEC (%p)",
                    ptr_flags,
                    ptr_registers,
                    ptr_stack,
                    ptr_executable
                );
            }

            ~processor() {
                // Free processor memory
                free(memory);
            }

            /**
             * copy_executable (src, size)
             * src: Executable pointer to data that will be copied
             * size: Executable size
             */
            void copy_executable(void* src, int size);

            /**
             * copy_executable (std::vector<executable_data_type> src)
             * src: Executable data that will be copied
             */
            void copy_executable(std::vector<executable_data_type> src);

        private:
            uint8_t* memory;

            /* Pointers to important memory positions */
            memory::flags* ptr_flags;
            register_type* ptr_registers;
            uint64_t* ptr_stack;
            executable_data_type* ptr_executable;
        };
    }
}