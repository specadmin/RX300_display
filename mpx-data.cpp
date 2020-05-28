//-----------------------------------------------------------------------------
#include "avr-debug/debug.h"
#include "mpx-data.h"
#include "data.h"
//-----------------------------------------------------------------------------
extern BYTE_DATA    ignition;
extern BYTE_DATA    climateEnabled;
extern BYTE_DATA    insideTemp;
extern BYTE_DATA    outsideTemp;
extern BYTE_DATA    blowerSpeed;
extern WORD_DATA    blowMode;

extern BYTE_DATA    speed;
extern WORD_DATA    averageSpeed;
extern WORD_DATA    fuelRateTime;
extern WORD_DATA    fuelRateDist;
extern WORD_DATA    averageFuelRate;
extern void         tripModeChange();
//-----------------------------------------------------------------------------
void mpxReceiver(__unused BYTE size, const BYTE* mpx_buf)
{
    switch(mpx_buf[2])
    {
    case 0x15:
        if(mpx_buf[3] & 0x01)
        {
            tripModeChange();
        }
        break;
    case 0x20:
        fuelRateTime = 778; // mpx_buf[3] << 8 | mpx_buf[4];
        break;
    case 0x24:
        speed = 90; // mpx_buf[3];
        ignition = mpx_buf[4] & 0x80;
        break;
    case 0x25:
        averageSpeed = 684; //mpx_buf[3] << 8 | mpx_buf[4];
        break;
    case 0x90:
        fuelRateDist = 998; //mpx_buf[3] << 8 | mpx_buf[4];
        break;
    case 0xA3:
        averageFuelRate = 107;// mpx_buf[3] << 8 | mpx_buf[4];
        break;
    case 0xCD:
        outsideTemp = mpx_buf[3];
        break;
    case 0x1B:
        blowerSpeed = mpx_buf[3];
        break;
    case 0xFC:
        blowMode = mpx_buf[3] << 8 | mpx_buf[4];
        break;
    case 0xFD:
        if(mpx_buf[3] < 0x38)
        {
            climateEnabled = 1;
            insideTemp = mpx_buf[3];
        }
        else
        {
            climateEnabled = 0;
        }
        break;
    }
}
//-----------------------------------------------------------------------------
