#include "menu.h"
#include "draw.h"
#include "goldhen.h"
#include <stdio.h>
#include <string.h>

MenuState g_menu = {
    .colors = { 0xE0101018, 0xFFFFFFFF, 0xFF3FA9F5 },
    .aim = { .fov = 150.0f, .smooth = 5.0f, .bone = 0 },
    .visuals = { .box = true, .snapline = true }
};

static const char *tabs[] = { "Aim", "Visuals", "Exploits", "Config" };

void menu_update(void) {
    static uint64_t last_toggle = 0;
    uint64_t now = goldhen_uptime_ms();

    if (goldhen_button_pressed(BTN_R1) && goldhen_button_pressed(BTN_SQUARE)) {
        if (now - last_toggle > 250) { 
            g_menu.open = !g_menu.open; 
            if (!g_menu.open) menu_save();
            last_toggle = now; 
        }
    }
    if (!g_menu.open) return;

    if (goldhen_button_pressed(BTN_RIGHT)) g_menu.tab = (g_menu.tab + 1) % 4;
    if (goldhen_button_pressed(BTN_LEFT))  g_menu.tab = (g_menu.tab + 3) % 4;
    if (goldhen_button_pressed(BTN_DOWN))  g_menu.cursor++;
    if (goldhen_button_pressed(BTN_UP))    g_menu.cursor--;
    if (g_menu.cursor < 0) g_menu.cursor = 0;
    
    if (goldhen_button_pressed(BTN_CROSS)) {
        if (g_menu.tab == 0) g_menu.aim.enabled = !g_menu.aim.enabled;
        if (g_menu.tab == 1) g_menu.visuals.box = !g_menu.visuals.box;
        if (g_menu.tab == 2) g_menu.exploits.no_spread = !g_menu.exploits.no_spread;
    }
}

void menu_draw(void) {
    if (!g_menu.open) return;
    draw_begin();

    int x = 40, y = 40, w = 600, h = 400;
    draw_rect(x, y, w, h, g_menu.colors.bg);
    draw_rect(x, y, w, 40, g_menu.colors.accent);
    draw_text(x + 10, y + 12, "FORTNITE PRX // v1.0", 0xFF000000, 2);

    int tx = x + 10;
    for (int i = 0; i < 4; i++) {
        if (i == g_menu.tab) draw_rect(tx, y + 45, 100, 25, g_menu.colors.accent);
        draw_text(tx + 10, y + 50, tabs[i], g_menu.colors.text, 1);
        tx += 110;
    }

    if (g_menu.tab == 0) {
        draw_text(x + 20, y + 90, g_menu.aim.enabled ? "[X] Memory Aim" : "[ ] Memory Aim", 0xFFFFFFFF, 1);
        draw_text(x + 20, y + 110, g_menu.aim.silent ? "[X] Silent Aim" : "[ ] Silent Aim", 0xFFFFFFFF, 1);
    } else if (g_menu.tab == 2) {
        draw_text(x + 20, y + 90, g_menu.exploits.no_spread ? "[X] No Spread" : "[ ] No Spread", 0xFFFFFFFF, 1);
        draw_text(x + 20, y + 110, g_menu.exploits.inf_ammo ? "[X] Infinite Ammo" : "[ ] Infinite Ammo", 0xFFFFFFFF, 1);
    }

    draw_text(x + 10, y + h - 25, "R1+Square: Close | D-Pad: Nav | Cross: Toggle", 0xFFFFFFFF, 1);
    draw_commit();
}

void menu_save(void) { /* goldhen_write_file("/data/FortnitePRX.json", &g_menu, sizeof(g_menu)); */ }
void menu_load(void) { /* goldhen_read_file("/data/FortnitePRX.json", &g_menu, sizeof(g_menu)); */ }
