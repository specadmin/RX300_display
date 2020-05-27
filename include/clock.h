#ifndef TIME_INCLUDED
#define TIME_INCLUDED
//-----------------------------------------------------------------------------
#include "lib/RTD2660AVR/OSD.h"
#include "improvedOSD.h"
//-----------------------------------------------------------------------------
void RTC_init();
void alarms_receiever(BYTE alarms);
void displayClock();
//-----------------------------------------------------------------------------
extern CImprovedAreaSet     disp_clockH;
extern CImprovedAreaSet     disp_clockM;
extern CFontAreaSet         disp_clockColon;
//-----------------------------------------------------------------------------
#endif
