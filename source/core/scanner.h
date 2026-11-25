#pragma once
#include <stdint.h>

typedef struct {
    const char *name;
    const char *sig;
    int         rip_disp;
    uintptr_t   resolved;
    int         encrypted;
} SigSlot;

uintptr_t sig_scan(const char *module, const char *sig);
uintptr_t scan_reference(uintptr_t target, int range);
void resolve_all_slots(void);

extern SigSlot g_sigs[];
extern int g_sig_count;
