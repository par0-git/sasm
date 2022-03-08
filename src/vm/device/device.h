/**
 * SemiAssembly / sasm virtual machine code
 * @file device.h
 * @author lotuspar / par0-git
 * @brief Base memory device class for other devices to branch from
 */

#pragma once

#include "../../any/number.h" // u*, vptr
#include "../../any/debug.h" // sasm_*
#include "status.h" // MemoryDeviceStatus
#include <string.h> // strlen, memcpy?

namespace sasm {
namespace vm {
    class MemoryDevice {
    public:
        MemoryDevice() : size (0) {}
        MemoryDevice(const char* uid, u64 _size) {
            set_uid(uid);
            size = _size;
        }

        /**
         * @brief Set MemoryDevice identifier

         * @param new_uid New unique identifier to give to the MemoryDevice
         */
        void set_uid(const char* new_uid) {
            // Set identifier after checking provided string
            u16 identifier_len = strlen(new_uid) + 1; // Increment by 1 to include null terminator (\0)

            // Limit identifier if too long
            if (identifier_len > 32)
                identifier_len = 32;

            memcpy(uid, new_uid, identifier_len);
        }

        /**
         * @brief Read a specified amount of bytes from the provided location into the output pointer
         * 
         * @param offset Position in device memory to operate at
         * @param amount Amount of bytes to read from memory
         * @param output Output location to read bytes to
         * @return (u8 / DeviceOperationResult) Result of operation
         */
        u8 read(vptr offset, u64 amount, void* output) {
            // Validate arguments before running bare operation
            // Check if read-locked
            if (check(MemoryDeviceStatus::kReadLocked)) {
                sasm_debug_print("Tried reading from locked MemoryDevice. [%s]", uid);
                return DeviceOperationResult::kLocked;
            }

            // Check if operation OOB
            if (check(MemoryDeviceStatus::kSafe)) {
                if (offset + amount < 0 || offset + amount > size) {
                    sasm_debug_print("Operation OOB (offset %llu, device size %llu, read end %llu)", offset, size, offset + amount);
                    return DeviceOperationResult::kOutOfBounds;
                }
            }

            // Run bare operation
            bare_read(offset, amount, output);

            // Assume success
            return DeviceOperationResult::kSuccess;
        }

        /**
         * @brief Write the specified amount of bytes from the input pointer to the provided location
         * 
         * @param offset Position in device memory to operate at
         * @param amount Amount of bytes to write to memory
         * @param input Output location to read bytes from
         * @return (u8 / DeviceOperationResult) Result of operation
         */
        u8 write(vptr offset, u64 amount, void* input) {
            // Validate arguments before running bare operation
            // Check if write-locked
            if (check(MemoryDeviceStatus::kWriteLocked)) {
                sasm_debug_print("Tried writing to locked MemoryDevice. [%s]", uid);
                return DeviceOperationResult::kLocked;
            }
                
            // Check if operation OOB
            if (check(MemoryDeviceStatus::kSafe)) {
                if (offset + amount < 0 || offset + amount > size) {
                    sasm_debug_print("Operation OOB (offset %llu, device size %llu, read end %llu)", offset, size, offset + amount);
                    return DeviceOperationResult::kOutOfBounds;
                }
            }

            // Run bare operation
            bare_write(offset, amount, input);

            // Assume success
            return DeviceOperationResult::kSuccess;
        }

        /**
         * @brief Easier way to write to device memory using a type rather than a pointer to a value
         * 
         * @tparam T Value type to write to memory
         * @param offset Position in device memory to operate at
         * @param value Value to write into memory
         */
        template <class T = u8>
        void write_type(vptr offset, T value) {
            write(offset, sizeof(T), &value);
        }

        /**
         * @brief Easier way to read from device memory returning a type rather than using a pointer to an output
         * 
         * @tparam T Value type to read from memory
         * @param offset Position in device memory to operate at
         * @return T Value read from memory
         */
        template <class T = u8>
        T read_type(vptr position) {
            T output;
            read(position, sizeof(T), &output);
            return output;
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
        virtual void bare_read(vptr offset, u64 amount, void* output) = 0;
        virtual void bare_write(vptr offset, u64 amount, void* input) = 0;
    };
}
}