#ifndef TRIPCOMP_INCLUDED
#define TRIPCOMP_INCLUDED
//-----------------------------------------------------------------------------
#include "avr-misc/avr-misc.h"
#include "lib/RTD2660AVR/OSD.h"
#include "improvedOSD.h"
#include "data.h"
//-----------------------------------------------------------------------------
void calculateAverageSpeed();
void displayTripInfo();
void tripModeChange();
//-----------------------------------------------------------------------------
extern CFontAreaSet         disp_tripComp;
extern CFontArea*           disp_tripCompCaption1;
extern CFontArea*           disp_tripCompCaption2;
extern CImprovedAreaSet     disp_tripCompValue;
extern CFontArea*           disp_tripCompUnit;

extern BYTE_DATA            speed;
extern WORD_DATA            tripDistance;
extern WORD_DATA            dayDistance;
extern WORD_DATA            totalDriveDistance;
extern WORD_DATA            averageSpeed;
extern WORD_DATA            fuelRateTime;
extern WORD_DATA            fuelRateDist;
extern WORD_DATA            averageFuelRate;
extern WORD_DATA            totalDriveDistance;
extern DWORD_DATA           totalDriveTime;
extern WORD_DATA            totalDriveTimeMinutes;
extern WORD_DATA            tripTimer;
extern BYTE_DATA            tripDay;
//-----------------------------------------------------------------------------
#endif
