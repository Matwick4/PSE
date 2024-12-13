#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include "PSE_project.h"
#include "display/charHelper.h"


//register define
#define DISPLAY_WIDTH               128
#define DISPLAY_HIGHT               64
#define NUM_PAGES                   8

#define BUFF_LENGHT                 DISPLAY_WIDTH*NUM_PAGES

#define CONTROL_BYTE                0x80
#define DATA_BYTE                   0x40

#define SET_CONTRAST                0x81
#define SET_DISPLAY_OFF             0xAE
#define SET_DISPLAY_ON              0xAF
#define SET_MEM_ADRESS_MODE         0x20
#define SET_COLUMN_START_ADDR       0x21
#define SET_PAGE_START_ADDR         0x22
#define SET_DISPLAY_START_LINE_0    0x40
#define SET_SEG_0                   0xA0
#define SET_SEG_127                 0xA1
#define SET_MUX_RATIO               0xA8
#define SET_COM_NORMAL_DIR          0xC8
#define SET_DISPLAY_OFFSET          0xD3
#define SET_COM_PIN_HARDWARE        0xDA
#define SET_DISPLAY_CLK_DIVIDE      0xD5
#define SET_PRECHARGE_PERIOD        0xD9
#define SET_VCOM_DESEL_LVL          0xDB
#define SET_ENTIRE_ON               0xA4


//function definition
void sendCommand(uint8_t cmd);
void sendCommandList(uint8_t* buf,int len);
static void render();
void displayInit();
void clearDisplay();
void writeString(const char* string);
int getIndex(uint8_t);


#endif