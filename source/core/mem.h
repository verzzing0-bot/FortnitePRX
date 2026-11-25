#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool mem_read(uintptr_t addr, void *out, size_t size);
bool mem_write(uintptr_t addr, const void *in, size_t size);
uintptr_t mem_read_ptr(uintptr_t addr);
float mem_read_float(uintptr_t addr);
void mem_write_float(uintptr_t addr, float val);
