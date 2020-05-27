#ifndef TRIPCOMP_INCLUDED
#define TRIPCOMP_INCLUDED
//-----------------------------------------------------------------------------
#include "avr-misc/avr-misc.h"
#include "lib/RTD2660AVR/OSD.h"
#include "improvedOSD.h"
//-----------------------------------------------------------------------------
void displayTripInfo();
//-----------------------------------------------------------------------------
extern CFontAreaSet        disp_tripComp;
extern CFontArea*          disp_tripCompCaption;
extern CImprovedAreaSet    disp_tripCompValue;
extern CFontArea*          disp_tripCompUnit;
//-----------------------------------------------------------------------------
#endif
