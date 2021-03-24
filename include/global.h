#ifndef GLOBAL_INCLUDED
#define GLOBAL_INCLUDED
//-----------------------------------------------------------------------------
#include "avr-misc/avr-misc.h"
#include "lib/RTD2660AVR/OSD.h"
#include "improvedOSD.h"
//-----------------------------------------------------------------------------
void displayGlobal();
//-----------------------------------------------------------------------------
enum DISP_MODE
{
    DISP_MODE_NORMAL,
    DISP_MODE_BRIGHTNESS,
    DISP_MODE_VIDEO
};
//-----------------------------------------------------------------------------
extern CFontArea*       disp_buttons[8];
extern BYTE             displayMode;
extern WORD_DATA        timer;
//-----------------------------------------------------------------------------
#endif
