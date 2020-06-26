#ifndef BODY_INCLUDED
#define BODY_INCLUDED
//-----------------------------------------------------------------------------
#include "avr-misc/avr-misc.h"
#include "lib/RTD2660AVR/OSD.h"
#include "improvedOSD.h"
//-----------------------------------------------------------------------------
void hideCar();
void showCar();
//-----------------------------------------------------------------------------
extern CFontAreaSet        disp_car;
extern CFontAreaSet        disp_doors;
extern CFontArea*          disp_door_FL;
extern CFontArea*          disp_door_BL;
extern CFontArea*          disp_door_FBL;
extern CFontArea*          disp_door_FR;
extern CFontArea*          disp_door_BR;
extern CFontArea*          disp_door_FBR;
extern CFontAreaSet        disp_fuel;
extern CFontArea*          disp_fuel_value;
extern CFontAreaSet        disp_tire[4];
extern CFontArea*          disp_tire_value[4];
extern BYTE_DATA           ignition;
extern BYTE_DATA           selectorAT;
//-----------------------------------------------------------------------------
#endif
