#include "PSE_project.h"

void I2C_Init(uint16_t freqKHz)
{
    i2c_init(i2c_default, freqKHz * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));
}
void UART_Init()
{
  uart_init(GPS_UART,GPS_BAUDRATE);
  uart_set_format(GPS_UART,8,1,UART_PARITY_NONE);
  gpio_set_function(GPS_UART_TX,GPIO_FUNC_UART);
  gpio_set_function(GPS_UART_RX,GPIO_FUNC_UART);
  uart_set_fifo_enabled(GPS_UART,true);
}
void INIT_HW()
{
  //I2C bus init
  I2C_Init(400);
  UART_Init();
  displayInit();
  //init_gps();
}
int main()
{
    stdio_init_all();
    
    float lon;
    float lat;
    printf("HW INIT\n");
    // while (1)
    // {
        
    //     if(gpsHasFix())
    //     {
    //         lon=getLon();
    //         lat=getLat();
    //     }
    //     else
    //     {
    //       printf("GPS don't have fix");
    //     }
    //     printf("Trying FIX!\n");
    //     sleep_ms(1000);
    // }
    
}
