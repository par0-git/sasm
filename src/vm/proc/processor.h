/**
 * SemiAssembly / sasm virtual machine code
 * @file processor.h
 * @author lotuspar / par0-git
 * @brief Main processor class
 */

#include "../device/container.h"
#include "../device/types/pure.h"

namespace sasm {
namespace vm {
    /**
     * @brief Bitflags for processor management
     */
    enum ProcessorStatus {
        akHeader = 0b10000000, // "kHeader" Signifies that the memory device exists, helpful for iteration
        akWriteLocked = 0b01000000, // "kWriteLocked" Disallows writing to the memory device
        akReadLocked = 0b00100000, // "kReadLocked" Disallows reading from the memory device
        akManaged = 0b00010000, // "kManaged" Makes the memory device "managed", where it is fully handled & automated
        akManagedActivity1 = 0b00001000, // "kManagedActivity1" for use with a managed device, can be automatically set
        akManagedActivity2 = 0b00000100, // "kManagedActivity2" for use with a managed device, can be automatically set
        akSafe = 0b00001000 // "kSafe" Disallows out-of-bounds reading and writing. Enabled by default
    };

    class BaseProcessor {
    public:
        BaseProcessor() { init(16, 16); }
        BaseProcessor(u64 _stack, u64 _user) { init(_stack, _user); }

        void init(u64 _stack, u64 _user) {
            prm_flags = PureMemoryDevice("flags", 4);
            prm_stack = PureMemoryDevice("stack", _stack);
            prm_user = PureMemoryDevice("user", _user);

            prm.handle(&prm_flags)
                .handle(&prm_stack)
                .handle(&prm_user);


        }
    private:
        MemoryContainer prm;
        PureMemoryDevice prm_flags;
        PureMemoryDevice prm_stack;
        PureMemoryDevice prm_user;
    };

    class BasicInterpretedProcessor : BaseProcessor {

    };
}
}