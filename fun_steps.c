#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
// #include "hardware/uart.h"
#include <math.h>
#include "bmi160.h"
// #include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define LED_PIN 15
#define FREQ_HZ 10

float g_to_degrees(float g_value) {
    if (g_value > 1.0f) g_value = 1.0f;
    if (g_value < -1.0f) g_value = -1.0f;
    return asinf(g_value) * 180.0f / (float)M_PI;
}

float getMagnitude(float x, float y, float z) {
    return sqrtf(x * x + y * y + z * z);
}

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

    while (1) {
        float x, y, z;
        bmi160_read_accel(I2C_PORT, BMI160_ADDR, &x, &y, &z);
        
        // float angle_x = g_to_degrees(x);
        // float angle_y = g_to_degrees(y);
        // float angle_z = g_to_degrees(z);
        
        // printf("\rX: %.2fg (%.1f°) Y: %.2fg (%.1f°) Z: %.2fg (%.1f°)",
        //     x, angle_x, y, angle_y, z, angle_z);
        // printf("\nX: %.2fg (%.1f°) Y: %.2fg (%.1f°) Z: %.2fg (%.1f°)",
        //     x, angle_x, y, angle_y, z, angle_z);
        
        float magnitude = getMagnitude(x, y, z);
        printf("\rMagnitude: %.2f g", magnitude);
        if (magnitude > 1.0f) {
            gpio_put(LED_PIN, 1);
            printf(" (Over 1g)\n");
            sleep_ms(1000);
        } else {
            gpio_put(LED_PIN, 0);
            printf("\n");
        }

        sleep_ms(300 / FREQ_HZ);
    }
}