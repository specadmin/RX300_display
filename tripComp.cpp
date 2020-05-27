//-----------------------------------------------------------------------------
#include <stdio.h>
#include "avr-debug/debug.h"
#include "tripComp.h"
#include "data.h"
//-----------------------------------------------------------------------------
#define KMPH    "\004\005\006\007"
#define L100KM  "\003\006\002\001\001\004\005"
//-----------------------------------------------------------------------------
enum
{
    TRIP_TIME,
    TRIP_SPEED,
    TRIP_AVG_SPEED,
    TRIP_FUEL,
    TRIP_AVG_FUEL
};
//-----------------------------------------------------------------------------
static BYTE         tripMode = TRIP_TIME;
static char         string[10];
BYTE_DATA           vehicleSpeed;
WORD_DATA           averageVehicleSpeed;
CFontAreaSet        disp_tripComp;
CFontArea*          disp_tripCompCaption;
CImprovedAreaSet    disp_tripCompValue;
CFontArea*          disp_tripCompUnit;
extern BYTE_DATA    ignition;
//-----------------------------------------------------------------------------
void displayTripInfo()
{
    register BYTE tmp;
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
    switch(tripMode)
    {
    case TRIP_AVG_SPEED:
    case TRIP_TIME:
    case TRIP_FUEL:
    case TRIP_AVG_FUEL:
    case TRIP_SPEED:
        disp_tripCompCaption->print("SP");
        disp_tripCompUnit->print(KMPH);
        if(vehicleSpeed.updated)
        {
            vehicleSpeed.updated = 0;
            tmp = vehicleSpeed.value;
            sprintf(string, "%d", tmp);
            disp_tripCompValue.print(string);

        }
    }
}
//-----------------------------------------------------------------------------
