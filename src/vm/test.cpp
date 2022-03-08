#include "memory.h"
#include <iostream>

void abc(void* a) {
    if (a==0x0)
        return;
}

struct test_struct {
    float abc;
    int def;
    u8* foo;
    u64* foo_2;
    u32* foo_3;
    int ghi;
};

int main() {
    using namespace sasm::vm;
    MemoryContainer container;
    PureMemoryDevice cpu("proc", 4);
    PureMemoryDevice user("user", 1 * 1024 * 1024 * 1024);
    PureMemoryDevice sys("sys", 4);

    container.handle(&cpu);
    container.handle(&user);
    container.handle(&sys);

    container.write_type<u64>(2, 123412341234);
    std::cout << container.read_type<u64>(2);
    return 0;
}