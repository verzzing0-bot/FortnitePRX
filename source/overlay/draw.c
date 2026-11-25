#include "draw.h"
#include "font8x8.h"
#include "goldhen.h"
#include <string.h>

static uint32_t *g_fb;
static int g_w, g_h, g_pitch;
static int dx0, dy0, dx1, dy1;

void draw_init(void) {
    goldhen_get_framebuffer(&g_fb, &g_w, &g_h, &g_pitch);
    dx0 = g_w; dy0 = g_h; dx1 = 0; dy1 = 0;
}

void draw_begin(void) {
    dx0 = g_w; dy0 = g_h; dx1 = 0; dy1 = 0;
}

static inline void mark(int x, int y, int w, int h) {
    if (x < dx0) dx0 = x;
    if (y < dy0) dy0 = y;
    if (x + w > dx1) dx1 = x + w;
    if (y + h > dy1) dy1 = y + h;
}

void draw_rect(int x, int y, int w, int h, uint32_t c) {
    mark(x, y, w, h);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            g_fb[(y + i) * (g_pitch / 4) + (x + j)] = c;
}

void draw_rect_outline(int x, int y, int w, int h, int t, uint32_t c) {
    for (int i = 0; i < t; i++) {
        draw_rect(x, y + i, w, 1, c);
        draw_rect(x, y + h - 1 - i, w, 1, c);
        draw_rect(x + i, y, 1, h, c);
        draw_rect(x + w - 1 - i, y, 1, h, c);
    }
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t c) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = (dx > dy ? dx : dy);
    if (steps == 0) { draw_rect(x1, y1, 1, 1, c); return; }
    float xi = dx / (float)steps, yi = dy / (float)steps;
    float x = x1, y = y1;
    for (int i = 0; i <= steps; i++, x += xi, y += yi)
        draw_rect((int)x, (int)y, 1, 1, c);
}

void draw_text(int x, int y, const char *s, uint32_t c, int scale) {
    for (; *s; s++, x += 8 * scale) {
        int idx = (unsigned char)*s - 32;
        if (idx < 0 || idx >= 95) continue;
        for (int row = 0; row < 8; row++) {
            uint8_t bits = font8x8_basic[idx][row];
            for (int col = 0; col < 8; col++)
                if (bits & (1 << col))
                    draw_rect(x + col * scale, y + row * scale, scale, scale, c);
        }
    }
}

int text_width(const char *s, int scale) { return strlen(s) * 8 * scale; }

void draw_commit(void) {
    if (dx1 <= dx0 || dy1 <= dy0) return;
    goldhen_fb_lock();
    for (int y = dy0; y < dy1; y++)
        memcpy(&g_fb[y * (g_pitch / 4) + dx0], &g_fb[y * (g_pitch / 4) + dx0], (dx1 - dx0) * 4);
    goldhen_fb_unlock();
}
