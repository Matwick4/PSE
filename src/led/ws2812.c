#include "led/ws2812.h"

void put_pixel(PIO pio, uint sm, uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio,sm,pixel_grb<<8u);
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return  ( (uint32_t)(r)<<8 ) | 
            ( (uint32_t)(g)<<16) | 
            ( (uint32_t)(b));
}
void led_init(PIO* pio, uint* sm, uint* offset,uint pin)
{
    bool succes= pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, pio,sm,offset,pin,1,true);
    hard_assert(succes);

    ws2812_program_init(*pio,*sm,*offset,pin,800000,false);
}

void led_test(PIO pio, uint sm, uint num_pixel)
{
    for(int i=0;i<num_pixel;i++)
        put_pixel(pio,sm,urgb_u32(0x0f,0x0f,0x0f));
    sleep_ms(250);
    for(int i=0;i<num_pixel;i++)
        put_pixel(pio,sm,urgb_u32(0x00,0x00,0x00));
    sleep_ms(250);
}