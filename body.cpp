//-----------------------------------------------------------------------------
#include "avr-debug/debug.h"
#include "data.h"
#include "body.h"
//-----------------------------------------------------------------------------
BYTE_DATA           ignition;
CFontAreaSet        disp_car;
CFontAreaSet        disp_doors;
CFontArea*          disp_door_FL;
CFontArea*          disp_door_BL;
CFontArea*          disp_door_FBL;
CFontArea*          disp_door_FR;
CFontArea*          disp_door_BR;
CFontArea*          disp_door_FBR;
CFontAreaSet        disp_fuel;
CFontArea*          disp_fuel_value;
CFontAreaSet        disp_tire[4];
CFontArea*          disp_tire_value[4];
//-----------------------------------------------------------------------------
void onIgnitionOff()
{

    disp_fuel.hide();
    disp_fuel_value->hide();
    // disp_media_set.hide();
}
//-----------------------------------------------------------------------------
void hideCar()
{
    disp_car.hide();
    for(BYTE i = 0; i < 4; i++)
    {
        disp_tire[i].hide();
        disp_doors.hide();
    }
}
//-----------------------------------------------------------------------------
