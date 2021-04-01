#include "processor.h"
using namespace sasm::vm;

void processor::copy_executable(void* src, int size) {
    memcpy(ptr_executable, src, size);
}

void processor::copy_executable(std::vector<executable_data_type> src) {
    copy_executable(src.data(), src.size());
}