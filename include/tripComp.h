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
extern CFontArea*          disp_tripCompCaption1;
extern CFontArea*          disp_tripCompCaption2;
extern CImprovedAreaSet    disp_tripCompValue;
extern CFontArea*          disp_tripCompUnit;
//-----------------------------------------------------------------------------
#endif
