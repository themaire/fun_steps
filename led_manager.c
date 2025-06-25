#include "ws2812.pio.h"
#include <stdlib.h>
#include <time.h>

#include "led_manager.h"

void init_leds(PIO pio, uint sm, uint pin) {
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, pin, 800000, IS_RGBW);
}

void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

static inline uint32_t urgbw_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            ((uint32_t) (w) << 24) |
            (uint32_t) (b);
}

void set_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

uint32_t rgb_to_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

uint32_t rgbw_to_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | ((uint32_t)(w) << 24) | (uint32_t)(b);
}

void display_pattern(PIO pio, uint sm, uint len, uint t, void (*pattern)(PIO, uint, uint, uint)) {
    pattern(pio, sm, len, t);
}

void pattern_snakes(PIO pio, uint sm, uint len, uint t) {
    for (uint i = 0; i < len; ++i) {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel(pio, sm, urgb_u32(0xff, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(pio, sm, urgb_u32(0, 0xff, 0));
        else if (x >= 30 && x < 40)
            put_pixel(pio, sm, urgb_u32(0, 0, 0xff));
        else
            put_pixel(pio, sm, 0);
    }
}

void pattern_random(PIO pio, uint sm, uint len, uint t) {
    if (t % 8)
        return;
    for (uint i = 0; i < len; ++i)
        put_pixel(pio, sm, rand());
}

void pattern_sparkle(PIO pio, uint sm, uint len, uint t) {
    if (t % 8)
        return;
    for (uint i = 0; i < len; ++i)
        put_pixel(pio, sm, rand() % 16 ? 0 : 0xffffffff);
}

void pattern_greys(PIO pio, uint sm, uint len, uint t) {
    uint max = 100; // let's not draw too much current!
    t %= max;
    for (uint i = 0; i < len; ++i) {
        put_pixel(pio, sm, t * 0x10101);
        if (++t >= max) t = 0;
    }
}

void pattern_k2000(PIO pio, uint sm, uint len, uint t) {
    static int direction = 1;
    static int pos = 0;

    // Affiche la LED à la bonne position
    for (uint i = 0; i < len; ++i)
        put_pixel(pio, sm, (i == pos) ? urgb_u32(0xff, 0, 0) : 0);

    // Avance la position
    if (t % 2 == 0) {
        pos += direction;
        if (pos >= (int)len - 1) {
            pos = len - 1;
            direction = -1;
        } else if (pos <= 0) {
            pos = 0;
            direction = 1;
        }
    }
}

void pattern_kitt(PIO pio, uint sm, uint len, uint t) {
    static int direction = 1;
    static int pos = 0;
    const int trail = 4; // longueur de la trainée

    for (uint i = 0; i < len; ++i) {
        int dist = abs((int)i - pos);
        if (dist == 0) {
            // LED principale, rouge vif
            put_pixel(pio, sm, urgb_u32(0xff, 0, 0));
        } else if (dist < trail) {
            // Trainée rouge plus faible
            uint8_t intensity = 255 / (dist + 1);
            put_pixel(pio, sm, urgb_u32(intensity, 0, 0));
        } else {
            // LED éteinte
            put_pixel(pio, sm, 0);
        }
    }

    // Avance la position
    if (t % 2 == 0) {
        pos += direction;
        if (pos >= (int)len - 1) {
            pos = len - 1;
            direction = -1;
        } else if (pos <= 0) {
            pos = 0;
            direction = 1;
        }
    }
}

void pattern_cop(PIO pio, uint sm, uint len, uint t) {
    // On suppose len = 16 (8 rouges, 8 bleues)
    bool swap = (t / 20) % 2; // alterne toutes les ~200ms si sleep_ms(10)
    for (uint i = 0; i < len; ++i) {
        if (i < len / 2) {
            // Première moitié
            put_pixel(pio, sm, swap ? urgb_u32(0, 0, 0xff) : urgb_u32(0xff, 0, 0));
        } else {
            // Deuxième moitié
            put_pixel(pio, sm, swap ? urgb_u32(0xff, 0, 0) : urgb_u32(0, 0, 0xff));
        }
    }
}
