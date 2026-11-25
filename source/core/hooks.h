#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void    *target;
    void    *detour;
    uint8_t  saved[32];
    int      stolen_size;
    void    *trampoline;
    bool     installed;
} Hook;

bool hook_install(Hook *h, void *target, void *detour);
bool hook_remove(Hook *h);
