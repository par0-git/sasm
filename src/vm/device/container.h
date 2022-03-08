/**
 * SemiAssembly / sasm virtual machine code
 * @file container.h
 * @author lotuspar / par0-git
 * @brief Container class to handle multiple MemoryDevice instances
 */

#pragma once

#include "device.h" // MemoryDevice
#include <vector> // std::vector

namespace sasm {
namespace vm {
    struct MemoryMapChild {
        MemoryDevice* device;
        u64 offset; // Address handled (virtual address) in MemoryContainer
    };

    class MemoryContainer : public MemoryDevice {
        public:
            // constructor: Creates a MemoryContainer
            MemoryContainer() {
                unset(MemoryDeviceStatus::kSafe); // Doesn't need Out-of-Bounds checking  
            }

            /**
             * @brief Make this container start handling a new device
             * 
             * @param device Pointer to MemoryDevice
             */
            MemoryContainer& handle(MemoryDevice* device) {
                devices.push_back(device); // Push pointer to vector
                remap(); // Remap container
                return *this; // Return self
            }

            /**
             * @brief Finds the MemoryDevice handling the provided container address
             * 
             * @param addr Container address to check for device
             * @param offset_out Offset from the beginning of the container memory space
             * @return Found MemoryDevice pointer, 0x0 for nothing
             */
            MemoryDevice* find(vptr addr, u64* offset_out = 0x0) {
                u64 offset = 0x0;
                for (u8 i = 0; i < devices.size(); i++) {
                    // device_start_boundary == offset
                    u64 device_end_boundary = offset + devices[i]->size; // Position (virtual) of last virtual address handled by device

                    // Check if provided address is within the space handled by the device
                    if (addr >= offset && addr <= device_end_boundary) {
                        if (offset_out != 0x0)
                            *offset_out = offset;

                        return devices[i];
                    }

                    // Add to offset
                    offset += devices[i]->size;
                }
                return 0x0;
            }

            /**
             * @brief Read an amount of bytes from the provided container address into the output pointer
             * 
             * @param position Container address to start operation at
             * @param amount Amount of bytes to read
             * @param output Pointer to read bytes to
             */
            void bare_read(vptr offset, u64 amount, void* output) {
                // operation_position: incremented as more devices are written to 
                u8* operation_position = (u8*) output;

                // Iterate over all device mappings
                for (u8 i = 0; i < mapping.size(); i++) {
                    u64 device_offset = 0;
                    u64 device_amount = multi_device_operation(mapping[i], offset, amount, &device_offset);
                    if (device_amount == 0)
                        continue; // Nothing should be done to this device

                    // Read from device
                    mapping[i].device->read(device_offset, device_amount, operation_position);
                    operation_position += device_amount;
                }
            }

            /**
             * @brief Write an amount of bytes to the provided container address from the input pointer
             * 
             * @param position Container address to start operation at
             * @param amount Amount of bytes to write
             * @param output Pointer to write bytes from
             */
            void bare_write(vptr offset, u64 amount, void* input) {
                // operation_position: incremented as more devices are written to 
                u8* operation_position = (u8*) input;

                // Iterate over all device mappings
                for (u8 i = 0; i < mapping.size(); i++) {
                    u64 device_offset = 0;
                    u64 device_amount = multi_device_operation(mapping[i], offset, amount, &device_offset);

                    if (device_amount == 0)
                        continue; // Nothing should be done to this device

                    // Write to device
                    mapping[i].device->write(device_offset, device_amount, operation_position);
                    operation_position += device_amount;
                }
            }
            
        private:
            std::vector<MemoryDevice*> devices;
            std::vector<MemoryMapChild> mapping;

            /**
             * @brief Internal function to help handle an operation spanning multiple MemoryDevices
             * 
             * This function returns a few values to help with the operation
             * 
             * @param child Memory container mapping child for the device
             * @param position Position in memory to start operation at
             * @param amount Amount of bytes for operation to handle
             * @param offset_out [OUT] Position into device memory operation should start at
             * @return u64 Amount of bytes for device to handle
             */
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

                // Offset is now the size of all devices combined
                size = offset;

                // Debug information
                sasm_debug_print("Remapped MemoryContainer devices (%lu devices)", mapping.size());
            }
    };
}
}