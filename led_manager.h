#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include "pico/stdlib.h" // Pour les fonctions d'allocation dynamique ici RANDOM pour les patterns de LEDS
#include "hardware/pio.h"

// Définition des constantes
#define NUM_PIXELS 16
#define IS_RGBW false

// Déclaration des fonctions
void init_leds(PIO pio, uint sm, uint pin);
void set_pixel(PIO pio, uint sm, uint32_t pixel_grb);
// Function prototype
void put_pixel(PIO pio, uint sm, uint32_t pixel_grb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
static inline uint32_t urgbw_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

uint32_t rgb_to_u32(uint8_t r, uint8_t g, uint8_t b);
uint32_t rgbw_to_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

void leds_clear(PIO pio, uint sm, uint len);

void display_pattern(PIO pio, uint sm, uint len, uint t, void (*pattern)(PIO, uint, uint, uint));

// Déclaration du type pattern
typedef void (*pattern)(PIO pio, uint sm, uint len, uint t);


// Déclaration des fonctions de motifs
void pattern_kitt(PIO pio, uint sm, uint len, uint t);
void pattern_cop(PIO pio, uint sm, uint len, uint t);
void pattern_snakes(PIO pio, uint sm, uint len, uint t);
void pattern_random(PIO pio, uint sm, uint len, uint t);
void pattern_sparkle(PIO pio, uint sm, uint len, uint t);
void pattern_greys(PIO pio, uint sm, uint len, uint t);
void pattern_k2000(PIO pio, uint sm, uint len, uint t);

// Déclaration externe de pattern_table
typedef struct {
    pattern pat;
    const char *name;
} PatternEntry;

extern const PatternEntry pattern_table[];

#endif // LED_MANAGER_H