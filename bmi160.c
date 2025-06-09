#include "bmi160.h"
#include "pico/stdlib.h"
#include <math.h>

static void write_reg(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c, addr, buf, 2, false);
}

static void read_regs(i2c_inst_t *i2c, uint8_t addr, uint8_t reg, uint8_t *buf, size_t len) {
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    i2c_read_blocking(i2c, addr, buf, len, false);
}

void bmi160_init(i2c_inst_t *i2c, uint8_t addr) {
    // Soft reset
    write_reg(i2c, addr, BMI160_REG_CMD, BMI160_CMD_SOFT_RESET);
    sleep_ms(100);

    // Accel normal mode
    write_reg(i2c, addr, BMI160_REG_CMD, BMI160_CMD_ACC_NORMAL);
    sleep_ms(100);

    // Accel config (ODR 100Hz, normal)
    write_reg(i2c, addr, BMI160_REG_ACC_CONF, 0x2C);
    // Accel range ±8g
    write_reg(i2c, addr, BMI160_REG_ACC_RANGE, 0x08);
}

uint8_t bmi160_who_am_i(i2c_inst_t *i2c, uint8_t addr) {
    uint8_t id;
    read_regs(i2c, addr, BMI160_REG_CHIP_ID, &id, 1);
    return id;
}

static float decode_accel(int16_t raw) {
    // ±8g, 4096 LSB/g
    return (float)raw / BMI160_LSB_PER_G_8G;
}

void bmi160_read_accel(i2c_inst_t *i2c, uint8_t addr, float *x, float *y, float *z) {
    uint8_t buf[6];
    read_regs(i2c, addr, BMI160_REG_DATA_ACC, buf, 6);
    int16_t raw_x = (buf[1] << 8) | buf[0];
    int16_t raw_y = (buf[3] << 8) | buf[2];
    int16_t raw_z = (buf[5] << 8) | buf[4];
    *x = decode_accel(raw_x);
    *y = decode_accel(raw_y);
    *z = decode_accel(raw_z);
}