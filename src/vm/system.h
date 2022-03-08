#pragma once
#include "../shared/number.h"
#include "../shared/debug.h"
#include "memory.h"

namespace sasm {
namespace vm {
    class Processor {
        public:
            Processor(u32 mem_stack, u32 mem_user) {
                Create(mem_stack, mem_user);
            }

            void Create(u32 mem_stack, u32 mem_user) {
                // Allocate memory for processor virtual memory
                u8* vptr_procmem = (u8*) calloc(1, 16);
                u8* vptr_stack = (u8*) calloc(1, mem_stack);
                u8* vptr_usermem = (u8*) calloc(1, mem_user);

                // Add memory to memory controller
                MemoryDevice* dvc_procmem = vmem.AddDevice(vptr_procmem, 16);
                dvc_procmem->status |= MemoryDeviceStatus::kWriteLocked; // Block writing to device

                u8 stack_allowed_permissions[] = {MemoryPermissionGroup::kStack};
                vmem.AddDevice(vptr_stack, mem_stack, stack_allowed_permissions, sizeof(stack_allowed_permissions));
                
                vmem.AddDevice(vptr_usermem, mem_user);

                // Testing
                void* outA;
                MemoryDevice* dvcA = vmem.GetDevice(0x0, &outA);

                void* outB;
                MemoryDevice* dvcB = vmem.GetDevice(0x32, &outB);

                void* outC;
                MemoryDevice* dvcC = vmem.GetDevice(0x800, &outC);

                sasm_print("0x0: [dvc] %p, [real] %p, [out] %p", dvcA, dvcA->ptr, outA);
                sasm_print("0x32: [dvc] %p, [real] %p, [out] %p", dvcB, dvcB->ptr, outB);
                sasm_print("0x800: [dvc] %p, [real] %p, [out] %p", dvcC, dvcC->ptr, outC);
            }
        private:
            MemoryContainer vmem;
    };
}
}