#ifndef MMA8451Q_H
#define MMA8451Q_H

#include "common.h"

#define STATUS_REGISTER     0x00
#define OUT_X_MSB           0x01
#define OUT_X_LSB           0x02
#define OUT_Y_MSB           0x03
#define OUT_Y_LSB           0x04
#define OUT_Z_MSB           0x05
#define OUT_Z_LSB           0x06
#define F_SETUP             0x09
#define TRIG_CFG            0x0A
#define XYZ_DATA_CFG        0x0E
#define HP_FILTER_CUTOFF    0x0F 
#define CTRL_REG1           0x2A
#define CTRL_REG2           0x2B
#define CTRL_REG3           0x2C
#define CTRL_REG4           0x2D
#define CTRL_REG5           0x2E 


#define ACTIVE              0b00000001
#define MASK_2G             0b00000011
#define MASK_4G             0b00000010
#define MASK_8G             0b00000001

void MMA8451Q_Init();
void MMA8451Q_Active();
void MMA8451Q_Standby();
void MMA8451Q_2gMode();
void MMA8451Q_4gMode();
void MMA8451Q_8gMode();



#endif