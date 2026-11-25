#include "scanner.h"
#include "goldhen.h"
#include <string.h>
#include <stdlib.h>

static inline uintptr_t decrypt_world(uintptr_t a) {
    return a ^ 0xFFFFFFFFD599D092ULL;
}

SigSlot g_sigs[] = {
    { "PostRenderTable", "78 57 50 12 ?? ?? ?? ?? 00 08 57 50 12 ?? ?? ?? ??", -1, 0, 0 },
    { "UWorld",    "48 8B 05 ?? ?? ?? ?? 48 8B 88 ?? ?? ?? ?? 48 85 C9", 3, 0, 1 },
    { "GObjects",  "48 8B 05 ?? ?? ?? ?? 48 8B 88 ?? ?? ?? ?? 48 85 C9", 3, 0, 0 },
    { "LocalPawn", "48 8B 05 ?? ?? ?? ?? 48 8B 88 ?? ?? ?? ?? 48 85 C9", 3, 0, 0 }
};
int g_sig_count = sizeof(g_sigs) / sizeof(g_sigs[0]);

static bool match_at(const uint8_t *p, const char *sig, int *out_len) {
    int i = 0;
    const char *s = sig;
    while (*s) {
        if (*s == ' ') { s++; continue; }
        if (s[0] == '?' && s[1] == '?') { i++; s += 2; continue; }
        uint8_t want = (uint8_t)strtoul((char[]){s[0], s[1], 0}, NULL, 16);
        if (p[i] != want) return false;
        i++; s += 2;
    }
    *out_len = i;
    return true;
}

uintptr_t sig_scan(const char *module, const char *sig) {
    uintptr_t base = 0, size = 0;
    if (!goldhen_module_range(module, &base, &size)) return 0;
    const uint8_t *mem = (const uint8_t *)base;
    for (size_t off = 0; off + 32 < size; off++) {
        int len;
        if (match_at(mem + off, sig, &len)) return base + off;
    }
    return 0;
}

uintptr_t scan_reference(uintptr_t target, int range) {
    uintptr_t base = 0, size = 0;
    if (!goldhen_module_range("eboot.bin", &base, &size)) return 0;
    const uint8_t *p = (const uint8_t *)base;
    
    for (size_t off = 0; off + 8 < size; off++) {
        bool is_mov = (p[off] == 0x48 && p[off+1] == 0x8B && (p[off+2] & 0xC7) == 0x05);
        bool is_call = (p[off] == 0xFF && p[off+1] == 0x15);
        if (is_mov || is_call) {
            int32_t disp = *(int32_t *)(p + off + (is_call ? 2 : 3));
            uintptr_t resolved = (uintptr_t)(p + off + (is_call ? 6 : 7)) + disp;
            if (resolved >= target - range && resolved <= target + range)
                return base + off;
        }
    }
    return 0;
}

void resolve_all_slots(void) {
    for (int i = 0; i < g_sig_count; i++) {
        uintptr_t hit = sig_scan("eboot.bin", g_sigs[i].sig);
        if (!hit) continue;
        if (g_sigs[i].rip_disp >= 0) {
            int32_t disp = *(int32_t *)(hit + g_sigs[i].rip_disp);
            uintptr_t raw = hit + g_sigs[i].rip_disp + 4 + disp;
            g_sigs[i].resolved = g_sigs[i].encrypted ? decrypt_world(raw) : raw;
        } else {
            g_sigs[i].resolved = hit;
        }
    }
}
