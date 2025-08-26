#include "accelerometer/MMA8451Q.h"


void MMA8451Q_Init()
{
    MMA8451Q_Standby();

    //Put eventual config of register

    MMA8451Q_Active();
}


void MMA8451Q_Standby()
{
    uint8_t byte;


    i2c_read_blocking(i2c_default, CTRL_REG1,&byte,1,false);
    byte=byte&(~ACTIVE);
    i2c_write_blocking(i2c_default,CTRL_REG1,&byte,1,false);
}

void MMA8451Q_Active()
{
    uint8_t byte;

    i2c_read_blocking(i2c_default,CTRL_REG1, &byte,1,false);
    byte|= ACTIVE;
    i2c_write_blocking(i2c_default,CTRL_REG1,&byte,1,false);
}

void MMA8451Q_2gMode()
{
    MMA8451Q_Standby();

    uint8_t byte;
    i2c_read_blocking(i2c_default,XYZ_DATA_CFG,&byte,1,false);
    byte= byte&(~MASK_2G);
    i2c_write_blocking(i2c_default,XYZ_DATA_CFG,&byte,1,false);

    MMA8451Q_Active();
}

void MMA8451Q_4gMode()
{
    MMA8451Q_Standby();
     
    uint8_t byte;
    i2c_read_blocking(i2c_default,XYZ_DATA_CFG,&byte,1,false);
    byte= byte&(~MASK_4G);
    i2c_write_blocking(i2c_default,XYZ_DATA_CFG,&byte,1,false);

    MMA8451Q_Active();
}

void MMA8451Q_8gMode()
{
    MMA8451Q_Standby();

    uint8_t byte;
    i2c_read_blocking(i2c_default,XYZ_DATA_CFG,&byte,1,false);
    byte= byte&(~MASK_8G);
    i2c_write_blocking(i2c_default,XYZ_DATA_CFG,&byte,1,false);

    MMA8451Q_Active();
}