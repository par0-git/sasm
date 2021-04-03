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
            processor(int executable_size, int stack_size, void* existing_memory = 0x0, int provided_identifier = 0) {
                // Set identifier
                identifier = provided_identifier;

                // Calculate size
                int size = sizeof(memory::flags) + // Size of processor flags
                            (sizeof(register_type) * register_enum::xx) + // Space for processor registers
                            stack_size + // Space for stack
                            executable_size + // Space for executable
                            4; // 4 byte padding
                            
                sasm_print("Attempting to allocate %i bytes for virtual processor memory", size);

                // Make sure stack can hold at least 1 register
                if (sizeof(register_type) > stack_size) {
                    sasm_print("Stack size (%i) is not large enough (%lu) for a single register. Aborting.", stack_size, sizeof(register_type));
                    throw std::invalid_argument("Processor memory error");
                }

                // Make sure executable space can hold executable header
                if (sizeof(executable_header) > executable_size) {
                    sasm_print("Executable space (%lu) is not large enough for a header. Aborting.", sizeof(executable_header));
                    throw std::invalid_argument("Processor memory error");
                }

                // Attempt to allocate memory
                if (existing_memory == 0x0) {
                    memory = (uint8_t*) malloc(size);   
                } else {
                    memory = existing_memory;
                    sasm_print("Using provided memory address. PTR (%p)", existing_memory);
                }

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

                // Allow execution
                exec_active();
            }

            ~processor() {
                // Free processor memory
                free(memory);
            }

            /**
             * copy_executable (void* src, int size)
             * src: Executable pointer to data that will be copied
             * size: Executable size
             */
            void copy_executable(void* src, int size);
 
            /**
             * copy_executable (std::vector<executable_data_type> src)
             * src: Executable data that will be copied
             */
            void copy_executable(std::vector<executable_data_type> src);

            /**
             * process ()
             * Does one step of execution
             */
            void process();

            /**
             * run ()
             * Does all steps of execution while running and until halted
             */
            void run();

            /**
             * return_memory_ptr ()
             * RETURN: void*
             */
            void* return_memory_ptr() {
                return memory;
            }

            /**
             * return_identifier ()
             * RETURN: int
             */
            int return_identifier() {
                return identifier;
            }
             
        private:
            void* memory;
            int identifier;

            /* Pointers to important memory positions */
            memory::flags* ptr_flags;
            register_type* ptr_registers;
            uint64_t* ptr_stack;
            executable_data_type* ptr_executable;

            /**
             * exec_stop_active ()
             * Halt execution of the processor and stops it completely
             */
            void exec_stop_active();

            /**
             * exec_active ()
             * Allows execution of the processor
             */
            void exec_active();

            /**
             * exec_check_active ()
             * Check if processor execution is allowed
             * RETURN: bool, processor status
             */
            bool exec_check_active();

            /**
             * exec_wait ()
             * Freeze execution of the processor
             */
            void exec_wait();

            /**
             * exec_stop_wait ()
             * Continue execution of the processor
             */
            void exec_stop_wait();

            /**
             * exec_check_waiting ()
             * Check if processor is frozen
             * RETURN: bool, processor frozen status
             */
            bool exec_check_waiting();

            /* Register functions (These are very annoying & confusing to name.) */
            template <class provided_register_type>
            provided_register_type& register_get_value(register_enum r) {
                /**
                 * ptr_registers + (sizeof(register_type) * r) / Get pointer 
                 *
                 */ 

                (provided_register_type*)(ptr_registers + (sizeof(register_type) * r))
            }
        };
    }
}