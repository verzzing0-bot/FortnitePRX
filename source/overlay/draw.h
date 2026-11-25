#pragma once
#include <stdint.h>

void draw_init(void);
void draw_begin(void);
void draw_commit(void);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_rect_outline(int x, int y, int w, int h, int t, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void draw_text(int x, int y, const char *text, uint32_t color, int scale);
int  text_width(const char *text, int scale);
