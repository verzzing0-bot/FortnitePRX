#include "goldhen.h"
#include "core/hooks.h"
#include "core/scanner.h"
#include "overlay/menu.h"
#include "overlay/draw.h"

GOLDHEN_PLUGIN_NAME("FortnitePRX");
GOLDHEN_PLUGIN_VERSION("1.0.0");
GOLDHEN_PLUGIN_AUTHOR("Lunyx");

static Hook g_post_render_hook;
static void *g_post_render_orig;

extern void aim_tick(void);
extern void aim_init(void);
extern void visuals_draw(void);
extern void exploits_tick(void);

static void postrender_detour(void *hud, void *canvas) {
    menu_update();
    exploits_tick();
    if (g_menu.open) menu_draw();
    visuals_draw();
    aim_tick();
    ((void (*)(void*, void*))g_post_render_orig)(hud, canvas);
}

void plugin_init(void) {
    draw_init();
    menu_load();
    
    uintptr_t table = sig_scan("eboot.bin", "78 57 50 12 ?? ?? ?? ?? 00 08 57 50 12 ?? ?? ?? ??");
    uintptr_t ref = scan_reference(table, 4096);
    uintptr_t fn = ref; // FILL: Run backward prologue search here
    
    if (fn && hook_install(&g_post_render_hook, (void*)fn, postrender_detour)) {
        g_post_render_orig = g_post_render_hook.trampoline;
    }

    resolve_all_slots();
    aim_init();
}

void plugin_fini(void) {
    hook_remove(&g_post_render_hook);
    menu_save();
}
