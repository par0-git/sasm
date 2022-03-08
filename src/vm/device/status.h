/**
 * SemiAssembly / sasm virtual machine code
 * @file status.h
 * @author lotuspar / par0-git
 * @brief Enums for various device status codes
 */

#pragma once

namespace sasm {
namespace vm {
    /**
     * @brief Bitflags for memory device management
     */
    enum MemoryDeviceStatus {
        kHeader = 0b10000000, // "kHeader" Signifies that the memory device exists, helpful for iteration
        kWriteLocked = 0b01000000, // "kWriteLocked" Disallows writing to the memory device
        kReadLocked = 0b00100000, // "kReadLocked" Disallows reading from the memory device
        kManaged = 0b00010000, // "kManaged" Makes the memory device "managed", where it is fully handled & automated
        kManagedActivity1 = 0b00001000, // "kManagedActivity1" for use with a managed device, can be automatically set
        kManagedActivity2 = 0b00000100, // "kManagedActivity2" for use with a managed device, can be automatically set
        kSafe = 0b00001000 // "kSafe" Disallows out-of-bounds reading and writing. Enabled by default
    };

    /**
     * @brief Result codes for a device-specific operation
     */
    enum DeviceOperationResult {
        kSuccess = 0, // Operation succeeded
        kLocked = 1, // Failed due to device being locked "kWriteLocked" / "kReadLocked"
        kOutOfBounds = 2 // Failed, operation is not within device handled bounds and "kSafe" is on 
    };
}
}