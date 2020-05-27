#ifndef CLIMATE_INCLUDED
#define CLIMATE_INCLUDED
//-----------------------------------------------------------------------------
#include "lib/RTD2660AVR/OSD.h"
#include "improvedOSD.h"
//-----------------------------------------------------------------------------
#define DEG_C   "\003\004"
//-----------------------------------------------------------------------------
void displayClimate();
//-----------------------------------------------------------------------------
extern CFontAreaSet        disp_outsideTemp;
extern CImprovedAreaSet    disp_outsideTempValue;
extern CFontAreaSet        disp_insideTemp;
extern CFontArea*          disp_insideTempCaption;
extern CImprovedAreaSet    disp_insideTempValue;

extern CFontAreaSet        disp_brightness;
extern CFontAreaSet        disp_fan;
extern CFontAreaSet        disp_scaleHolder;
extern CFontAreaSet        disp_scale;
extern CFontAreaSet        disp_scaleValue[7];

extern CFontAreaSet        disp_body;
extern CFontAreaSet        disp_flowDirection;
extern CFontAreaSet        disp_windShield;
extern CFontAreaSet        disp_auto;
extern CFontAreaSet        disp_headArrow;
extern CFontAreaSet        disp_feetArrow;
//-----------------------------------------------------------------------------
#endif
