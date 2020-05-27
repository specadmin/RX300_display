//-----------------------------------------------------------------------------
#include "avr-debug/debug.h"
#include "mpx-data.h"
#include "data.h"
//-----------------------------------------------------------------------------
extern BYTE_DATA    ignition;
extern BYTE_DATA    climateEnabled;
extern BYTE_DATA    insideTemp;
extern BYTE_DATA    outsideTemp;
extern BYTE_DATA    vehicleSpeed;
extern BYTE_DATA    blowerSpeed;
extern WORD_DATA    blowMode;
extern WORD_DATA    averageVehicleSpeed;
//-----------------------------------------------------------------------------
void mpxReceiver(__unused BYTE size, const BYTE* mpx_buf)
{
    switch(mpx_buf[2])
    {
    case 0x24:
        vehicleSpeed = mpx_buf[3];
        ignition = mpx_buf[4] & 0x80;
        break;
    case 0x25:
        averageVehicleSpeed = mpx_buf[3] << 8 | mpx_buf[4];
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
