//-----------------------------------------------------------------------------
#include <stdio.h>
#include "avr-debug/debug.h"
#include "tripComp.h"
#include "units.h"
#include "body.h"
#include "options.h"
//-----------------------------------------------------------------------------
enum
{
    TRIP_MODE_TIME,
    TRIP_MODE_DISTANCE,
    TRIP_MODE_DAY_DISTANCE,
    TRIP_MODE_SPEED,
    TRIP_MODE_AVG_SPEED,
    TRIP_MODE_FUEL,
    TRIP_MODE_AVG_FUEL,
    tripModesCount
};
//-----------------------------------------------------------------------------
static const char   empty_string[] = "---";
static char         string[10];
static BYTE_DATA    tripMode = TRIP_MODE_TIME;
static bool         tripInit = false;
BYTE_DATA           speed = 0;
BYTE_DATA           tripDay = 32;   // день от даты для отслеживания перехода на следующие сутки
WORD_DATA           tripDistance = 0;
WORD_DATA           dayDistance = 0;
WORD_DATA           totalDriveDistance = 0;
WORD_DATA           tripTimer = 0;          //  sec
WORD_DATA           tripTimerMinutes = 0;
DWORD_DATA          totalDriveTime = 0;
WORD_DATA           averageSpeed = -1;
WORD_DATA           fuelRateTime = -1;
WORD_DATA           fuelRateDist = -1;
WORD_DATA           averageFuelRate = -1;

CFontAreaSet        disp_tripComp;
CFontArea*          disp_tripCompCaption1;
CFontArea*          disp_tripCompCaption2;
CImprovedAreaSet    disp_tripCompValue;
CFontArea*          disp_tripCompUnit;
//-----------------------------------------------------------------------------
void tripModeChange()
{
    tripMode = (tripMode.value < tripModesCount - 1) ? tripMode.value + 1 : 0;
}
//-----------------------------------------------------------------------------
void calculateAverageSpeed()
{
    if(totalDriveDistance.changed() && totalDriveTime > 60)
    {
        averageSpeed = (DWORD) totalDriveDistance * 3600 / totalDriveTime;
    }
}
//-----------------------------------------------------------------------------
__inline void display_trip_distance()
{
    sprintf(string, "%d", tripDistance.value);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_day_distance()
{
    sprintf(string, "%d", dayDistance.value);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_speed()
{
    if(options.convertKm2Miles)
    {
        sprintf(string, "%d", (WORD) speed.value * 10 / 16);
    }
    else
    {
        sprintf(string, "%d", speed.value);
    }
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_average_speed()
{
    if((int)averageSpeed.value >= 0)
    {
        sprintf(string, "%0d", averageSpeed.value);
        disp_tripCompValue.print(string);
    }
    else
    {
        disp_tripCompValue.print(empty_string);
    }
}
//-----------------------------------------------------------------------------
__inline void display_trip_time()
{
    sprintf(string, "%01d:%02d", (tripTimerMinutes.value / 60) %10, tripTimerMinutes.value % 60);
    disp_tripCompValue.print(string);
}
//-----------------------------------------------------------------------------
__inline void display_fuel_rate_time()
{
    if((int)fuelRateTime.value >= 0)
    {
        WORD rate = fuelRateTime.value;
        if(options.convertLitres2GalonsUSA)
        {
            rate = rate * 10 / 38;
        }
        if(options.convertLitres2GalonsENG)
        {
            rate = rate * 100 / 455;
        }
        sprintf(string, "%0d.%0d", rate / 100, rate % 100 / 10);
        disp_tripCompValue.print(string);
    }
    else
    {
        disp_tripCompValue.print(empty_string);
    }
}
//-----------------------------------------------------------------------------
__inline void display_fuel_rate_dist()
{
    static WORD rate;
    if((int)fuelRateDist.value > 0)
    {
        if(options.invertFuelRate)
        {
            rate = 10000 / fuelRateDist.value;
        }
        else
        {
            rate = fuelRateDist.value;
        }
        if(rate > 999) rate = 999;
        sprintf(string, "%0d.%0d", rate / 10, rate % 10);
        disp_tripCompValue.print(string);
    }
    else
    {
        disp_tripCompValue.print(empty_string);
    }
}
//-----------------------------------------------------------------------------
__inline void display_average_fuel_rate()
{
    static WORD rate;
    if((int)averageFuelRate.value >= 0)
    {
        if(options.invertFuelRate)
        {
            rate = 10000 / averageFuelRate.value;
        }
        else
        {
            rate = averageFuelRate.value;
        }
        if(rate > 999) rate = 999;
        sprintf(string, "%0d.%0d", rate / 10, rate % 10);
        disp_tripCompValue.print(string);
    }
    else
    {
        disp_tripCompValue.print(empty_string);
    }
}
//-----------------------------------------------------------------------------
void displayTripInfo()
{
    // cruise information visibility
    if(ignition)
    {
        disp_tripComp.show();
        disp_tripCompValue.show();
        tripInit = true;
        if(ignition.changed() && tripDay.changed())
        {
            dayDistance = 0;
        }
    }
    else
    {
        // TODO: сохранить trip в EEPROM при выключении зажигания
        disp_tripComp.hide();
        disp_tripCompValue.hide();
        tripTimer.value = 0;
    }

    switch(tripMode.value)
    {
    case TRIP_MODE_SPEED:
        if(tripMode.changed() || tripInit)
        {
            disp_tripCompCaption1->print("");
            disp_tripCompCaption2->print("SPEED");
            disp_tripCompUnit->print(options.speedUnits);
            display_speed();
        }
        if(speed.updated && speed.changed())
        {
            speed.updated = 0;
            display_speed();
        }
        break;
    case TRIP_MODE_DISTANCE:
        if(tripMode.changed() || tripInit)
        {
            disp_tripCompCaption1->print("TRIP");
            disp_tripCompCaption2->print("DIST");
            disp_tripCompUnit->print(options.distUnits);
            display_trip_distance();
        }
        if(tripDistance.updated && tripDistance.changed())
        {
            tripDistance.updated = 0;
            display_trip_distance();
        }
        break;
    case TRIP_MODE_DAY_DISTANCE:
        if(dayDistance.changed() || tripInit)
        {
            disp_tripCompCaption1->print("DAY");
            disp_tripCompCaption2->print("DIST");
            disp_tripCompUnit->print(options.distUnits);
            display_day_distance();
        }
        if(dayDistance.updated && dayDistance.changed())
        {
            dayDistance.updated = 0;
            display_day_distance();
        }
        break;
    case TRIP_MODE_AVG_SPEED:
        if(tripMode.changed() || tripInit)
        {
            disp_tripCompCaption1->print("AVG");
            disp_tripCompCaption2->print("SPEED");
            disp_tripCompUnit->print(options.speedUnits);
            display_average_speed();
        }
        if(averageSpeed.updated && averageSpeed.changed())
        {
            averageSpeed.updated = 0;
            display_average_speed();
        }
        break;
    case TRIP_MODE_TIME:
        if(tripMode.changed() || tripInit)
        {
            disp_tripCompCaption1->print("TRIP");
            disp_tripCompCaption2->print("TIME");
            disp_tripCompUnit->print("");
            display_trip_time();
        }
        if(tripTimer.updated)
        {
            tripTimer.updated = 0;
            tripTimerMinutes = tripTimer.value / 60;
        }
        if(tripTimerMinutes.changed())
        {
            display_trip_time();
        }
        break;
    case TRIP_MODE_FUEL:
        if(tripMode.changed() || tripInit)
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
            disp_tripCompUnit->print(options.fuelRateUnits);
            if(fuelRateDist.updated && fuelRateDist.changed())
            {
                fuelRateDist.updated = 0;
                display_fuel_rate_dist();
            }
        }
        else
        {
            disp_tripCompUnit->print(options.fuelRatePerTimeUnits);
            if(fuelRateTime.updated && fuelRateTime.changed())
            {
                fuelRateTime.updated = 0;
                display_fuel_rate_time();
            }
        }
        break;
    case TRIP_MODE_AVG_FUEL:
        if(tripMode.changed() || tripInit)
        {
            disp_tripCompCaption1->print("AVG");
            disp_tripCompCaption2->print("F/R");
            disp_tripCompUnit->print(options.fuelRateUnits);
            display_average_fuel_rate();
        }
        if(averageFuelRate.updated && averageFuelRate.changed())
        {
            averageFuelRate.updated = 0;
            display_average_fuel_rate();
        }
        break;
    }
    tripInit = false;
}
//-----------------------------------------------------------------------------
