#include "processor.h"
#include <thread>

int main() {
    std::vector<sasm::executable_data_type> executable = {
        0x10
    };

    sasm::vm::processor primary_processor(256, 256);
    sasm::vm::processor secondary_processor(256, 256, primary_processor.return_memory_ptr(), 1);

    primary_processor.copy_executable(executable);
    primary_processor.run();

    std::thread a(&sasm::vm::processor::run, std::ref(primary_processor));
    std::thread b(&sasm::vm::processor::run, std::ref(secondary_processor));

    return 0;
}