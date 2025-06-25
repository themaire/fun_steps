#ifndef BMI160_H
#define BMI160_H

#include "hardware/i2c.h"

#define BMI160_ADDR 0x69
#define BMI160_OK 0xD1

// Registres
#define BMI160_REG_CHIP_ID 0x00
#define BMI160_REG_CMD     0x7E
#define BMI160_REG_ACC_CONF 0x40
#define BMI160_REG_ACC_RANGE 0x41
#define BMI160_REG_DATA_ACC 0x12

// Commandes
#define BMI160_CMD_SOFT_RESET 0xB6
#define BMI160_CMD_ACC_NORMAL 0x11

void bmi160_init(i2c_inst_t *i2c, uint8_t addr);
uint8_t bmi160_who_am_i(i2c_inst_t *i2c, uint8_t addr);
void bmi160_read_accel(i2c_inst_t *i2c, uint8_t addr, float *x, float *y, float *z);

// Facteurs de conversion LSB/g selon la plage de mesure
#define BMI160_LSB_PER_G_2G   16384.0f
#define BMI160_LSB_PER_G_4G    8192.0f
#define BMI160_LSB_PER_G_8G    4096.0f
#define BMI160_LSB_PER_G_16G   2048.0f

// Deux fonctions utilitaires
float g_to_degrees(float g_value);
float getMagnitude(float x, float y, float z);

#endif