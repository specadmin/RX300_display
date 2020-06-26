//-----------------------------------------------------------------------------
#include "avr-debug/debug.h"
#include "mpx-data.h"
#include "data.h"
#include "options.h"
#include "tripComp.h"
#include "body.h"
//-----------------------------------------------------------------------------
extern BYTE_DATA    ignition;
extern BYTE_DATA    climateEnabled;
extern BYTE_DATA    insideTemp;
extern BYTE_DATA    outsideTemp;
extern BYTE_DATA    blowerSpeed;
extern WORD_DATA    blowMode;
WORD_DATA           distance = 0;
//-----------------------------------------------------------------------------
#define msg_id  mpx_buf[2]
#define byte0   mpx_buf[3]
#define byte1   mpx_buf[4]
#define byte2   mpx_buf[5]
//-----------------------------------------------------------------------------
void mpxReceiver(__unused BYTE size, const BYTE* mpx_buf)
{
    switch(msg_id)
    {
    case 0x15:
        if(byte0 & 0x01)
        {
            // temporary switch
            tripModeChange();
        }
        break;
    case 0x20:
        fuelRateTime = byte0 << 8 | byte1;
        break;
    case 0x24:
        speed = byte0;
        break;
    case 0x25:
        // averageSpeed = byte0 << 8 | byte1;
        if(size > 7)
        {
            // the packet has an extra byte with units
            switch(byte2)
            {
            case 0x40:
                options.speedUnits = units.MPH;
                options.convertKm2Miles = true;
                break;
            default:
                options.speedUnits = units.KMPH;
            }
        }
        break;
    case 0x40:
        selectorAT = byte0;
        break;
    case 0x6A:
        switch(byte0)
        {
        case 0x10:
            options.distUnits = units.MILES;
            options.fuelRateUnits = units.MPG;
            options.fuelRatePerTimeUnits = units.GPH;
            options.convertKm2Miles = true;
            options.convertLitres2GalonsUSA = true;
            break;
        case 0x11:
            options.invertFuelRate = false;
        }
        break;
    case 0x76:
        tripDistance = byte0 << 8 | byte1;
        distance = tripDistance;
        if(distance.changed() && distance.value)
        {
            dayDistance++;
            if(selectorAT & 0x1F)
            {
                totalDriveDistance++;
            }
        }
        if(size > 7)
        {
            // the packet has an extra byte with units
            options.distUnits = (byte2 == 0x40) ? units.MILES : units.KM;
        }
        break;
    case 0x90:
        fuelRateDist = byte0 << 8 | byte1;
        if(size > 7)
        {
            // the packet has an extra byte with units
            switch(byte2)
            {
            case 0x40:
                options.invertFuelRate = false;
            case 0x80:
                options.fuelRateUnits = units.L100KM;
                break;
            case 0x20:
                options.fuelRateUnits = units.MPG;
                options.fuelRatePerTimeUnits = units.GPH;
                options.convertLitres2GalonsUSA = true;
                break;
            case 0x10:
                options.fuelRateUnits = units.MPG;
                options.fuelRatePerTimeUnits = units.GPH;
                options.convertLitres2GalonsENG = true;
                break;
            }
        }
        break;
    case 0xA3:
        averageFuelRate = byte0 << 8 | byte1;
        break;
    case 0xCD:
        outsideTemp = byte0;
        break;
    case 0x1B:
        blowerSpeed = byte0;
        climateEnabled = (byte0 & 0x10) ? 1 : 0;
        break;
    case 0xFC:
        blowMode = byte0 << 8 | byte1;
        break;
    case 0xDF:
        ignition = byte0 & 0x40;
        break;
    case 0xFD:
        if(byte0 < 0x38)
        {
            insideTemp = byte0;
        }
        break;
    }
}
//-----------------------------------------------------------------------------
