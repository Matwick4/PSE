#include "display/displayDriver.h"



static uint8_t memBuff[BUFF_LENGHT];//maybe add a variable to keep track where we arrived
static uint16_t memIndex = 0;

void sendCommand(uint8_t cmd)
{
    uint8_t buf[2]= {CONTROL_BYTE,cmd};

    i2c_write_blocking(i2c_default,I2C_DISPLAY_ADR,buf,2,false);
}

void sendCommandList(uint8_t* buf,int len)
{
    for (int i = 0; i < len; i++)
    {
        sendCommand(buf[i]);
    }
    
}

static void render()
{
    clearDisplay();
    uint8_t tmpBuf[BUFF_LENGHT+1];
    tmpBuf[0]=0x40;

    memcpy(tmpBuf+1,memBuff,BUFF_LENGHT);

    i2c_write_blocking(i2c_default,I2C_DISPLAY_ADR,tmpBuf,BUFF_LENGHT+1,false);
}

void clearDisplay()
{
    uint8_t tmpBuf[BUFF_LENGHT+1];
    tmpBuf[0]=0x40;
    memset(tmpBuf+1,0x00,BUFF_LENGHT);

    memIndex=0;

    i2c_write_blocking(i2c_default,I2C_DISPLAY_ADR,tmpBuf,BUFF_LENGHT+1,false);
}

void writeString(const char* buf)
{
    //write code to handle the \n char
    int len = strlen(buf);
    if(len > 64)// string to big
        return;

    uint8_t index;
    for (int i = 0; i < len; i++)
    {
       index=getIndex(toupper(buf[i]));
       for (int j = 0; j < 8; j++)
       {
            memBuff[memIndex++] = charHelper[index*8+j];
       }
    }

    render();
}

int getIndex(uint8_t c)
{
    if (c >= 'A' && c <='Z') {
            return  c - 'A' + 1;
        }
        else if (c >= '0' && c <='9') {
            return  c - '0' + 27;
        }
        else return  0; // Not got that char so space.
}
void displayInit()
{
    //full reset sequence
    //review something i can remove or write better
    uint8_t init[]={
      SET_DISPLAY_OFF,
      SET_MEM_ADRESS_MODE,
      0x00, // horizontal adress mode
      SET_DISPLAY_START_LINE_0,
      SET_SEG_127,
      SET_MUX_RATIO,
      0x3F,
      SET_COM_NORMAL_DIR,
      SET_DISPLAY_OFFSET,
      0x00,
      SET_COM_PIN_HARDWARE,
      0x02,
      SET_DISPLAY_CLK_DIVIDE,
      0x80,
      SET_PRECHARGE_PERIOD,
      0xF1,
      SET_VCOM_DESEL_LVL,
      0x30,
      SET_CONTRAST,
      0x7F,
      0x8D,
      0x14,
      SET_ENTIRE_ON,
      0xAF
    };
    sendCommandList(init,24);
} 
