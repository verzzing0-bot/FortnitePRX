#include "hooks.h"
#include "goldhen.h"
#include <string.h>

static int insn_len(const uint8_t *p) {
    if (p[0] == 0x55 || p[0] == 0x53 || p[0] == 0x56 || p[0] == 0x57) return 1;
    if (p[0] == 0x41 && (p[1] == 0x54 || p[1] == 0x55)) return 2;
    if (p[0] == 0x48 && p[1] == 0x83 && p[2] == 0xEC) return 4;
    if (p[0] == 0x48 && p[1] == 0x81 && p[2] == 0xEC) return 7;
    if (p[0] == 0x48 && p[1] == 0x89 && p[2] == 0x5C && p[3] == 0x24) return 5;
    if (p[0] == 0x48 && p[1] == 0x8B && p[2] == 0xC4) return 3;
    if (p[0] == 0x48 && p[1] == 0x8B && (p[2] & 0xC7) == 0x05) return 7;
    if (p[0] == 0xFF && p[1] == 0x15) return 6;
    if (p[0] == 0xC3 || p[0] == 0xCC) return 1;
    return 0;
}

static void emit_abs_jmp(uint8_t *p, uint64_t addr) {
    p[0] = 0xFF; p[1] = 0x25;
    *(uint32_t *)(p + 2) = 0;
    *(uint64_t *)(p + 6) = addr;
}

bool hook_install(Hook *h, void *target, void *detour) {
    h->target = target;
    h->detour = detour;
    uint8_t *src = (uint8_t *)target;
    int stolen = 0;

    while (stolen < 14) {
        int len = insn_len(src + stolen);
        if (len == 0 || stolen + len > 28) return false;
        stolen += len;
    }
    h->stolen_size = stolen;

    h->trampoline = goldhen_alloc_exec(64);
    if (!h->trampoline) return false;

    memcpy(h->saved, target, stolen);
    memcpy(h->trampoline, h->saved, stolen);
    emit_abs_jmp((uint8_t *)h->trampoline + stolen, (uint64_t)((uintptr_t)target + stolen));

    uint8_t patch[32];
    memset(patch, 0x90, stolen);
    emit_abs_jmp(patch, (uint64_t)detour);
    goldhen_write_mem(target, patch, stolen);

    h->installed = true;
    return true;
}

bool hook_remove(Hook *h) {
    if (!h->installed) return false;
    goldhen_write_mem(h->target, h->saved, h->stolen_size);
    h->installed = false;
    return true;
}
