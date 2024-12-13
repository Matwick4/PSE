#ifndef PSE_PROJECT_H
#define PSE_PROJECT_H

// HEADER FILE INCLUDE
#include <stdio.h>
#include <string.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"


// USER MADE HEADER FILE
#include "led/ws2812.h"
#include "led/ws2812.pio.h"
#include "display/displayDriver.h"


//PICO PIN DEFINE
#define PIO_LED 13
#define NUM_PIXEL 8
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

//I2C ADRESS DEFINE

#define I2C_DISPLAY_ADR 0x3C
#define I2C_ACC_ADDR    0x1D

#endif