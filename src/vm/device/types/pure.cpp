/**
 * SemiAssembly / sasm virtual machine code: device type
 * @file pure.cpp
 * @author lotuspar / par0-git
 * @brief PureMemoryDevice
 */

#include "pure.h"

using namespace sasm::vm;

void PureMemoryDevice::bare_read(vptr offset, u64 amount, void* output) {
    // Get pointer to device real address as u8* & add offset
    u8* device_real_u8p = ((u8*) real) + offset;
    u8* output_u8p = (u8*) output;

    // Copy data from device to output
    for (u64 x = 0; x < amount; x++) {
        output_u8p[x] = device_real_u8p[x];
    }
}

void PureMemoryDevice::bare_write(vptr offset, u64 amount, void* input) {
    // Get pointer to device real address as u8* & add offset
    u8* device_real_u8p = ((u8*) real) + offset;
    u8* input_u8p = (u8*) input;

    // Copy data from input to device
    for (u64 x = 0; x < amount; x++) {
        device_real_u8p[x] = input_u8p[x];
    }
}