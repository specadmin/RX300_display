//-----------------------------------------------------------------------------
#include "avr-debug/debug.h"
#include "data.h"
#include "global.h"
#include "body.h"
#include "RTD2660AVR/display.h"
//-----------------------------------------------------------------------------
CFontArea*          disp_buttons[8];
BYTE_DATA           selectorAT = 0;
WORD_DATA           timer = 1;          // in seconds
BYTE                displayMode = DISP_MODE_NORMAL;
//-----------------------------------------------------------------------------
void displayGlobal()
{
    static WORD R_timer = 1;
    if(ignition)
    {
        if(selectorAT.changed())
        {
            if(selectorAT.value == 0x40)
            {
                R_timer = timer + REAR_VIDEO_DELAY;
            }
            else
            {
                display.hideVideo();
                OSD.show(&OSD.maps[0]);
                displayMode = DISP_MODE_NORMAL;
                R_timer = 0;
            }
        }
        if(timer == R_timer)
        {
            OSD.hide();
            display.showVideo(VS_AV2);
            displayMode = DISP_MODE_VIDEO;
            R_timer = 0;
        }
    }
    else
    {
        display.hideVideo();
        OSD.show(&OSD.maps[0]);
        displayMode = DISP_MODE_NORMAL;
        R_timer = 0;
    }
}
//-----------------------------------------------------------------------------
