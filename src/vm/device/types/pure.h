/**
 * SemiAssembly / sasm virtual machine code: device type
 * @file pure.h
 * @author lotuspar / par0-git
 * @brief Definition of PureMemoryDevice
 * 
 * A PureMemoryDevice allocates a space in memory. Operating on the device acts
 * directly on the allocated memory. Writing to 0x0 of the device writes to the first
 * byte of allocated memory.
 */

#pragma once

#include "../../../any/number.h" // u*, vptr
#include "../../../any/debug.h" // sasm_*
#include "../device.h"
#include <string.h> // strlen, memcpy?
#include <stdlib.h>
#include <vector>

namespace sasm {
namespace vm {
    class PureMemoryDevice : public MemoryDevice {
    public:
        PureMemoryDevice(const char* _uid, u64 _size) {
            init(_uid, _size);
        }
        PureMemoryDevice() {}

        void init(const char* _uid, u64 _size) {
            set_uid(_uid);
            real = malloc(_size);
            size = _size;

            if (real == 0x0) {
                size = 0;
                sasm_print("Failed to initialize PureMemoryDevice, 0x0 malloc result");
            }
        }

        void bare_read(vptr offset, u64 amount, void* output);
        void bare_write(vptr offset, u64 amount, void* input);
    private:
        // Pointer to memory handled by the MemoryDevice
        void* real;
    };
}
}
