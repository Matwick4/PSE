#ifndef WS2812_H
#define WS2812_H

//TODO maybe put the PIO strucgt support here

#include "PSE_project.h"
#include "led/ws2812.pio.h"


void put_pixel(PIO pio, uint sm, uint32_t pixel_grb);
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void led_init(PIO* pio, uint* sm, uint* offset,uint pin);
void led_test(PIO pio, uint sm, uint num_pixel);

#endif
