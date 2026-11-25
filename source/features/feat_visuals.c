#include "../overlay/menu.h"
#include "../overlay/draw.h"
#include "../core/scanner.h"
#include "goldhen.h"

static bool w2s(float* matrix, float* pos, float* out, int width, int height) {
    float w = matrix[3] * pos[0] + matrix[7] * pos[1] + matrix[11] * pos[2] + matrix[15];
    if (w < 0.01f) return false;
    float x = matrix[0] * pos[0] + matrix[4] * pos[1] + matrix[8] * pos[2] + matrix[12];
    float y = matrix[1] * pos[0] + matrix[5] * pos[1] + matrix[9] * pos[2] + matrix[13];
    out[0] = (width / 2.0f) * (1.0f + x / w);
    out[1] = (height / 2.0f) * (1.0f - y / w);
    return true;
}

void visuals_draw(void) {
    if (!g_menu.visuals.box && !g_menu.visuals.snapline) return;
    // ESP loop
}
