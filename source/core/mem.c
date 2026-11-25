#include "mem.h"
#include "goldhen.h"

bool mem_read(uintptr_t addr, void *out, size_t size) {
    if (!addr) return false;
    return goldhen_read_mem((void *)addr, out, size);
}

bool mem_write(uintptr_t addr, const void *in, size_t size) {
    if (!addr) return false;
    return goldhen_write_mem((void *)addr, in, size);
}

uintptr_t mem_read_ptr(uintptr_t addr) {
    uintptr_t val = 0;
    mem_read(addr, &val, sizeof(val));
    return val;
}

float mem_read_float(uintptr_t addr) {
    float val = 0.0f;
    mem_read(addr, &val, sizeof(val));
    return val;
}

void mem_write_float(uintptr_t addr, float val) {
    mem_write(addr, &val, sizeof(val));
}
