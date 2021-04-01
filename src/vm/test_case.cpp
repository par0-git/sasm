#include "processor.h"

int main() {
    std::vector<sasm::executable_data_type> executable = {
        0x10
    };

    sasm::vm::processor processor(256, 256);
    processor.copy_executable(executable);
    return 0;
}