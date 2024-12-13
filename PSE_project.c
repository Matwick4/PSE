#include "PSE_project.h"



int main()
{
    //maybe put PIO in led header file
    PIO pio;
    uint sm;
    uint offset;
    stdio_init_all();

    //maybe write a function to init I2C
    //or some utils.h header file if needed
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));

    displayInit();
    clearDisplay();
    writeString("ABC");
    
    while (1)
    {
      
    }
}
