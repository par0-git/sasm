/**
 * SemiAssembly / sasm shared code
 * @file number.h
 * @author lotuspar / par0-git
 * @brief Definitions for numbers used to make coding simpler
 */

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Should be used instead of pointers when handling virtual memory
typedef uint64_t vptr;