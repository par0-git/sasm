#include "processor.h"
using namespace sasm::vm;

void processor::exec_active() {
    // Set flags.execution active bit to true
    ptr_flags->execution |= memory::execution_status::active;
}

void processor::exec_stop_active() {
    // Set flags.execution active bit to false
    ptr_flags->execution &= memory::execution_status::active;
}

bool processor::exec_check_active() {
    // Returns status of flags.execution active bit
    return ptr_flags->execution & memory::execution_status::active;
}

void processor::exec_wait() {
    // Set flags.execution waiting bit to false
    ptr_flags->execution &= memory::execution_status::waiting;
}

void processor::exec_stop_wait() {
    // Set flags.execution waiting bit to true
    ptr_flags->execution |= memory::execution_status::waiting;
}

bool processor::exec_check_waiting() {
    // Returns status of flags.execution waiting bit
    return ptr_flags->execution & memory::execution_status::waiting;
}

void processor::process() {
    printf("processing ");
}

void processor::run() {
    // Warn if execution isn't allowed
    if (!exec_check_active()) {
        sasm_print("Execution isn't allowed (flags.execution active bit != true) but run() was still called. Aborting.");
        return;
    }

    // Continue to run while flags.execution first (0) bit is set
    while (exec_check_active()) {
        if (exec_check_waiting()) // Don't process if processor is frozen
            continue;

        process();
    }
}