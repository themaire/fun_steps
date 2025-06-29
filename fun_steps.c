#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
// #include "hardware/uart.h"
#include <math.h>
#include "bmi160.h"

#include "ws2812.pio.h"
#include "led_manager.h"

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define LED_PIN 15
#define FREQ_HZ 10

#define IS_RGBW false
#define NUM_PIXELS 16

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 28
#endif

// Check the pin is compatible with the platform
#if WS2812_PIN >= NUM_BANK0_GPIOS
#error Attempting to use a pin>=32 on a platform that does not support it
#endif

// Définition de pattern_table qui liste des effets de LED disponibles
const PatternEntry pattern_table[] = {
    {pattern_kitt,   "K2000"},
    {pattern_cop,    "Cop Car"},
    {pattern_snakes, "Snakes!"},
    {pattern_random, "Random data"},
    {pattern_sparkle,"Sparkles"},
    {pattern_greys,  "Greys"},
};

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    sleep_ms(1000);

    bmi160_init(I2C_PORT, BMI160_ADDR);

    uint8_t id = bmi160_who_am_i(I2C_PORT, BMI160_ADDR);
    printf("ID capteur: 0x%02X\n", id);

    // Set pio and state machine for LED control
    PIO pio;
    uint sm;
    uint offset;

    // This will find a free pio and state machine for our program and load it for us
    // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
    // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    while (1) {
        float x, y, z;
        bmi160_read_accel(I2C_PORT, BMI160_ADDR, &x, &y, &z);
        
        // float angle_x = g_to_degrees(x);
        float angle_y = g_to_degrees(y);
        // float angle_z = g_to_degrees(z);
        
        // printf("\rX: %.2fg (%.1f°) Y: %.2fg (%.1f°) Z: %.2fg (%.1f°)",
        //     x, angle_x, y, angle_y, z, angle_z);
        
        float magnitude = getMagnitude(x, y, z);
        printf("\rMagnitude: %.2f g", magnitude);
        // if (magnitude > 1.0f) {
        //     gpio_put(LED_PIN, 0);
        //     printf(" (Over 1g)\n");
        //     sleep_ms(1000);
        // } else {
        //     gpio_put(LED_PIN, 1);
        //     printf("\n");
        // }

        // Afficher un motif de LEDs
        if (magnitude > 5.0f) {
            // Si l'accélération dépasse 1g, on affiche un motif
            play_pattern_by_name(pio, sm, NUM_PIXELS, "K2000", 60);
        } else if (magnitude > 2.0f) {
            // Sinon, on affiche un motif aléatoire
            play_pattern_by_name(pio, sm, NUM_PIXELS, "Snakes!", 60);
        } else if (angle_y > 40) {
            play_pattern_by_name(pio, sm, NUM_PIXELS, "Cop Car", 30);
        } else {
            printf("\n");
        }

        sleep_ms(300 / FREQ_HZ);
    }
}