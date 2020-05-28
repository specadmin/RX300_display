//-----------------------------------------------------------------------------
#include <stdio.h>
#include "avr-debug/debug.h"
#include "tripComp.h"
#include "data.h"
//-----------------------------------------------------------------------------
#define KMPH    "\004\005\006\007"
#define L100KM  "\003\006\002\001\001\004\005"
#define LPH     "\003\006\007"
//-----------------------------------------------------------------------------
enum
{
    TRIP_MODE_TIME,
    TRIP_MODE_SPEED,
    TRIP_MODE_AVG_SPEED,
    TRIP_MODE_FUEL,
    TRIP_MODE_AVG_FUEL,
    tripModesCount
};
//-----------------------------------------------------------------------------
static char         string[10];
static BYTE_DATA    tripMode = TRIP_MODE_TIME;
BYTE_DATA           speed = 0;
WORD_DATA           tripTimer = 0;
WORD_DATA           averageSpeed = 0;
WORD_DATA           fuelRateTime = 0;
WORD_DATA           fuelRateDist = 0;
WORD_DATA           averageFuelRate = 0;

CFontAreaSet        disp_tripComp;
CFontArea*          disp_tripCompCaption1;
CFontArea*          disp_tripCompCaption2;
CImprovedAreaSet    disp_tripCompValue;
CFontArea*          disp_tripCompUnit;
extern BYTE_DATA    ignition;
//-----------------------------------------------------------------------------
void tripModeChange()
{
    tripMode = (tripMode.value < tripModesCount - 1) ? tripMode.value + 1 : 0;
}
//-----------------------------------------------------------------------------
__inline void display_speed()
{
    sprintf(string, "%d", speed.value);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_average_speed()
{
    sprintf(string, "%0d.%0d", averageSpeed.value / 10, averageSpeed.value % 10);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_trip_time()
{
    sprintf(string, "%01d%01d", tripTimer.value / 60, tripTimer.value % 60);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_fuel_rate_time()
{
    sprintf(string, "%0d.%0d", fuelRateTime.value / 10, fuelRateTime.value % 10);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_fuel_rate_dist()
{
    sprintf(string, "%0d.%0d", fuelRateDist.value / 10, fuelRateDist.value % 10);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_average_fuel_rate()
{
    sprintf(string, "%0d.%0d", averageFuelRate.value / 10, averageFuelRate.value % 10);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
void displayTripInfo()
{
    // cruise information visibility
    if(ignition)
    {
        disp_tripComp.show();
        disp_tripCompValue.show();
    }
    else
    {
        // TODO: сохранить trip в EEPROM при выключении зажигания
        disp_tripComp.hide();
        disp_tripCompValue.hide();
    }
    switch(tripMode.value)
    {
    case TRIP_MODE_SPEED:
        if(tripMode.changed())
        {
            disp_tripCompCaption1->print("");
            disp_tripCompCaption2->print("SPEED");
            disp_tripCompUnit->print(KMPH);
            display_speed();
        }
        if(speed.updated && speed.changed())
        {
            speed.updated = 0;
            display_speed();
        }
        break;
    case TRIP_MODE_AVG_SPEED:
        if(tripMode.changed())
        {
            disp_tripCompCaption1->print("AVG");
            disp_tripCompCaption2->print("SPEED");
            disp_tripCompUnit->print(KMPH);
            display_average_speed();
        }
        if(averageSpeed.updated && averageSpeed.changed())
        {
            averageSpeed.updated = 0;
            display_average_speed();
        }
        break;
    case TRIP_MODE_TIME:
        if(tripMode.changed())
        {
            disp_tripCompCaption1->print("TRIP");
            disp_tripCompCaption2->print("TIME");
            disp_tripCompUnit->print("");
            display_trip_time();
        }
        if(tripTimer.updated && tripTimer.changed())
        {
            tripTimer.updated = 0;
            display_trip_time();
        }
        break;
    case TRIP_MODE_FUEL:
        if(tripMode.changed())
        {
            disp_tripCompCaption1->print("FUEL");
            disp_tripCompCaption2->print("RATE");
            if(speed > 0)
            {
                display_fuel_rate_dist();
            }
            else
            {
                display_fuel_rate_time();
            }
        }
        if(speed > 0)
        {
            disp_tripCompUnit->print(L100KM);
            if(fuelRateDist.updated && fuelRateDist.changed())
            {
                fuelRateDist.updated = 0;
                display_fuel_rate_dist();
            }
        }
        else
        {
            disp_tripCompUnit->print(LPH);
            if(fuelRateTime.updated && fuelRateTime.changed())
            {
                fuelRateTime.updated = 0;
                display_fuel_rate_time();
            }
        }
        break;
    case TRIP_MODE_AVG_FUEL:
        if(tripMode.changed())
        {
            disp_tripCompCaption1->print("AVG");
            disp_tripCompCaption2->print("F/R");
            disp_tripCompUnit->print(L100KM);
            display_average_fuel_rate();
        }
        if(averageFuelRate.updated && averageFuelRate.changed())
        {
            averageFuelRate.updated = 0;
            display_average_fuel_rate();
        }
        break;
    }
}
//-----------------------------------------------------------------------------
