#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool open;
    int tab;
    int cursor;
    struct {
        uint32_t bg, text, accent;
    } colors;
    struct {
        bool enabled, silent, triggerbot;
        int bone; float fov, smooth;
    } aim;
    struct {
        bool box, snapline, skeleton, distance, name;
    } visuals;
    struct {
        bool no_spread, rapid_fire, speed_hack, inf_ammo, no_recoil;
        bool shoot_through_walls, hitbox_expand;
        // Add all 22 features here
    } exploits;
} MenuState;

extern MenuState g_menu;

void menu_update(void);
void menu_draw(void);
void menu_save(void);
void menu_load(void);
