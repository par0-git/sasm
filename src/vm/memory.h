#pragma once
#include "../shared/number.h"
#include "../shared/debug.h" // sasm_print
#include <string.h> // strlen, memcpy?
#include <stdlib.h>
#include <vector>

/*
    u64 / uint64_t used for virtual addresses
    u8* / pointers used for real addresses

    Virtual Address (vaddr / VADDR): Refers to an address used by a MemoryContainer or MemoryDevice
*/

namespace sasm {
namespace vm {
    enum MemoryDeviceStatus {
        kHeader = 0b10000000, // "kHeader" Signifies that the memory device exists, helpful for iteration
        kWriteLocked = 0b01000000, // "kWriteLocked" Disallows writing to the memory device
        kReadLocked = 0b00100000, // "kReadLocked" Disallows reading from the memory device
        kManaged = 0b00010000, // "kManaged" Makes the memory device "managed", where it is fully handled & automated
        kManagedActivity1 = 0b00001000, // "kManagedActivity1" for use with a managed device, can be automatically set
        kManagedActivity2 = 0b00000100, // "kManagedActivity2" for use with a managed device, can be automatically set
        kSafe = 0b00001000 // "kSafe" Disallows out-of-bounds reading and writing. Enabled by default
    };

    class MemoryDevice {
    public:
        MemoryDevice() : size (0) {}
        MemoryDevice(const char* _uid, u64 _size) {
            initialize(_uid);
            size = _size;
        }

        void initialize(const char* _uid) {
            // Set identifier after checking provided string
            u16 _identifier_len = strlen(_uid) + 1; // Increment by 1 to include null terminator (\0)

            // Limit identifier if too long
            if (_identifier_len > 32)
                _identifier_len = 32;

            memcpy(uid, _uid, _identifier_len);
        }

        void read(u64 offset, u64 amount, void* output) {
            // Validate arguments before running bare operation
            // Check if read-locked
            if (check(MemoryDeviceStatus::kReadLocked)) {
                sasm_debug_print("operation fail: Device locked");
                return;
            }

            // Check if operation OOB
            if (check(MemoryDeviceStatus::kSafe)) {
                if (offset + amount < 0 || offset + amount > size) {
                    sasm_print("operation fail: Operation OOB (offset %llu, device size %llu, read end %llu)", offset, size, offset + amount);
                    return;
                }
            }

            // Run bare operation
            bare_read(offset, amount, output);
        }

        void write(u64 offset, u64 amount, void* input) {
            // Validate arguments before running bare operation
            // Check if write-locked
            if (check(MemoryDeviceStatus::kWriteLocked)) {
                sasm_debug_print("operation fail: Device locked");
                return;
            }
                
            // Check if operation OOB
            if (check(MemoryDeviceStatus::kSafe)) {
                if (offset + amount < 0 || offset + amount > size) {
                    sasm_print("operation fail: Operation OOB (offset %llu, device size %llu, read end %llu)", offset, size, offset + amount);
                    return;
                }
            }

            // Run bare operation
            bare_write(offset, amount, input);
        }
        void set(MemoryDeviceStatus bit) {status |= bit;}
        void unset(MemoryDeviceStatus bit) {status &= ~(bit);}
        bool check(MemoryDeviceStatus bit) {return status & (1 << bit);}

        u8 uid[32]; 
        u64 size;
    private:
        // Current status of device
        u8 status = MemoryDeviceStatus::kHeader | MemoryDeviceStatus::kSafe;

        // Bare operations (operations without checks for validity)
        virtual void bare_read(u64 offset, u64 amount, void* output) = 0;
        virtual void bare_write(u64 offset, u64 amount, void* input) = 0;
    };

    class PureMemoryDevice : public MemoryDevice {
    public:
        PureMemoryDevice(const char* _uid, u64 _size) {
            initialize(_uid);
            real = malloc(_size);
            size = _size;

            if (real == 0x0) {
                size = 0;
                sasm_print("failed to initialize PureMemoryDevice, 0x0 malloc result");
            }
        }

        /**
         * Read a provided amount of bytes from the MemoryDevice
         * @param offset Location (virtual; in memory device) of read operation
         * @param amount Amount of bytes to read
         * @param output Pointer to output bytes to
         */
        void bare_read(u64 offset, u64 amount, void* output) {
            // Get pointer to device real address as u8* & add offset
            u8* device_real_u8p = ((u8*) real) + offset;
            u8* output_u8p = (u8*) output;

            // Copy data from device to output
            for (u64 x = 0; x < amount; x++) {
                output_u8p[x] = device_real_u8p[x];
            }
        }

        /**
         * Write a provided amount of bytes to the MemoryDevice
         * @param offset Location (virtual; in memory device) of write operation
         * @param amount Amount of bytes to write
         * @param output Pointer to copy bytes from
         */
        void bare_write(u64 offset, u64 amount, void* input) {
            // Get pointer to device real address as u8* & add offset
            u8* device_real_u8p = ((u8*) real) + offset;
            u8* input_u8p = (u8*) input;

            // Copy data from input to device
            for (u64 x = 0; x < amount; x++) {
                device_real_u8p[x] = input_u8p[x];
            }
        }
    private:
        // Pointer to memory handled by the MemoryDevice
        void* real;
    };

    struct MemoryMapChild {
        MemoryDevice* device;
        u64 offset; // Address handled (virtual address) in MemoryContainer
    };

    class MemoryContainer {
        public:
            // constructor: Creates a MemoryContainer
            MemoryContainer() {}

            void handle(MemoryDevice* device) {
                devices.push_back(device);
                remap();
            }

            void remap() {
                // Clear memory map
                mapping.clear();

                // Iterate over all devices and add them to the mapping
                u64 offset = 0x0;
                for (u8 i = 0; i < devices.size(); i++) {
                    // Create new child
                    MemoryMapChild child;
                    child.device = devices[i];
                    child.offset = offset;

                    // Add child to mapping
                    mapping.push_back(child);

                    // Increment offset
                    offset += devices[i]->size;
                }

                // Debug information
                sasm_print("remapped (%lu devices)", mapping.size());
            }

            MemoryDevice* find(u64 vaddr, u64* offset_out = 0x0) {
                u64 offset = 0x0;
                for (u8 i = 0; i < devices.size(); i++) {
                    // device_start_boundary == offset
                    u64 device_end_boundary = offset + devices[i]->size; // Position (virtual) of last virtual address handled by device

                    // Check if provided address is within the space handled by the device
                    if (vaddr >= offset && vaddr <= device_end_boundary) {
                        if (offset_out != 0x0)
                            *offset_out = offset;

                        return devices[i];
                    }

                    // Add to offset
                    offset += devices[i]->size;
                }
                return 0x0;
            }

            void read(u64 position, u64 amount, void* output) {
                // operation_position: incremented as more devices are written to 
                u8* operation_position = (u8*) output;

                // Iterate over all device mappings
                for (u8 i = 0; i < mapping.size(); i++) {
                    u64 device_offset = 0;
                    u64 device_amount = multi_device_operation(mapping[i], position, amount, &device_offset);
                    if (device_amount == 0)
                        continue; // Nothing should be done to this device

                    // Read from device
                    mapping[i].device->read(device_offset, device_amount, operation_position);
                    operation_position += device_amount;
                }
            }

            void write(u64 position, u64 amount, void* input) {
                // operation_position: incremented as more devices are written to 
                u8* operation_position = (u8*) input;

                // Iterate over all device mappings
                for (u8 i = 0; i < mapping.size(); i++) {
                    u64 device_offset = 0;
                    u64 device_amount = multi_device_operation(mapping[i], position, amount, &device_offset);

                    if (device_amount == 0)
                        continue; // Nothing should be done to this device

                    // Write to device
                    mapping[i].device->write(device_offset, device_amount, operation_position);
                    operation_position += device_amount;
                }
            }

            template <class T = u8>
            void write_type(u64 position, T value) {
                write(position, sizeof(T), &value);
            }

            template <class T = u8>
            T read_type(u64 position) {
                T output;
                read(position, sizeof(T), &output);
                return output;
            }
            
        private:
            std::vector<MemoryDevice*> devices;
            std::vector<MemoryMapChild> mapping;

            u64 multi_device_operation(MemoryMapChild& child, u64 position, u64 amount, u64* offset_out) {
                // Check if device mapping is related to operation
                // Make sure address isn't before what this device is meant to handle
                if (position > child.offset + child.device->size) 
                    return 0;

                // device_start_bounds / child.offset: first address handled by device
                // device_end_bounds: last address handled by device
                u64 device_end_bounds = child.offset + (child.device->size - 1);

                // operation_start_bounds / position: first address written to by operation
                // operation_end_bounds: last address written to by operation
                u64 operation_end_bounds = position + (amount - 1);

                // Check if device is surrounded
                // "Operation Start <= Device Start && Operation End >= Device End"
                if (position <= child.offset && operation_end_bounds >= device_end_bounds) {
                    if (offset_out != 0x0)
                        *offset_out = 0; // Operation should deal with whole device, so start at beginning

                    // Device is fully surrounded by operation, use whole size
                    return child.device->size;
                }

                // Check if operation start is surrounded
                // "Operation Start >= Device Start && Operation Start <= Device End"
                if (position >= child.offset && position <= device_end_bounds) {
                    if (offset_out != 0x0)
                        *offset_out = position - child.offset; // Operation is currently handling first device, so start at specified offset

                    // (Device End - Operation Start) + 1
                    u64 device_amount = (device_end_bounds - position) + 1;

                    // Cap amount to operation size
                    if (device_amount > amount)
                        device_amount = amount;

                    return device_amount;
                }

                // Check if operation end is surrounded
                // "Operation End >= Device Start && Operation End <= Device End"
                if (operation_end_bounds >= child.offset && operation_end_bounds <= device_end_bounds) {
                    if (offset_out != 0x0)
                        *offset_out = 0; // Operation should start at beginning
                    
                    // (Operation End - Device Start) + 1
                    return (operation_end_bounds - child.offset) + 1;
                }

                return 0;
            }
    };
}
}
