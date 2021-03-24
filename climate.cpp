//-----------------------------------------------------------------------------
#include <stdio.h>
#include "avr-misc/avr-misc.h"
#include "avr-debug/debug.h"
#include "data.h"
#include "body.h"
#include "global.h"
#include "climate.h"
//-----------------------------------------------------------------------------
static char         string[10];
BYTE_DATA           insideTemp;
BYTE_DATA           outsideTemp;
BYTE_DATA           climateEnabled;
BYTE_DATA           blowerSpeed;
WORD_DATA           blowMode;
CFontAreaSet        disp_outsideTemp;
CImprovedAreaSet    disp_outsideTempValue;
CFontAreaSet        disp_insideTemp;
CFontArea*          disp_insideTempCaption;
CImprovedAreaSet    disp_insideTempValue;
CFontAreaSet        disp_brightness;
CFontAreaSet        disp_fan;
CFontAreaSet        disp_scaleHolder;
CFontAreaSet        disp_scale;
CFontAreaSet        disp_scaleValue[7];
CFontAreaSet        disp_body;
CFontAreaSet        disp_flowDirection;
CFontAreaSet        disp_windShield;
CFontAreaSet        disp_auto;
CFontAreaSet        disp_headArrow;
CFontAreaSet        disp_feetArrow;
//-----------------------------------------------------------------------------
void displayClimate()
{
    static const char inTemp[][5] = {   "COLD", "16.5", "17.0", "17.5", "18.0", "18.5", "19.0", "19.5", "20.0", "20.5", "21.0",
                                        "21.5", "22.0", "22.5", "23.0", "23.5", "24.0", "24.5", "25.0", "25.5", "26.0", "26.5",
                                        "27.0", "27.5", "28.0", "28.5", "29.0", "29.5", "30.0", "30.5", "31.0", "31.5", "32.0",
                                        "32.5", "33.0", "33.5", "34.0", "34.5", "35.0", "35.5", "36.0", "36.5", "37.0", "37.5",
                                        "38.0", "38.5", "39.0" };
    register BYTE tmp;
    register WORD wtmp;

    // climate visibility
    if(!ignition)
    {
        disp_flowDirection.hide();
        disp_outsideTemp.hide();
        disp_outsideTempValue.hide();
        disp_brightness.hide();
        climateEnabled = 0;
    }

    // climate enabled
    if(climateEnabled.updated)
    {
        if(climateEnabled.value)
        {
            disp_insideTemp.show();
            disp_insideTempValue.show();
            if(displayMode == DISP_MODE_NORMAL)
            {
                disp_scaleHolder.show();
                disp_fan.show();
            }
        }
        else
        {
            disp_insideTemp.hide();
            disp_insideTempValue.hide();
            if(displayMode == DISP_MODE_NORMAL)
            {
                disp_fan.hide();
                disp_scale.hide();
            }
        }
    }
    // outside temp
    if(outsideTemp.updated)
    {
        outsideTemp.updated = 0;
        tmp = outsideTemp.value;
        if(tmp)
        {
            disp_outsideTemp.show();
            disp_outsideTempValue.show();
        }
        else
        {
            disp_outsideTemp.hide();
            disp_outsideTempValue.hide();
            return;
        }
        if(outsideTemp.changed())
        {
            sprintf(string, "%d", tmp - 48);
            disp_outsideTempValue.print(string);
        }
    }


    // target inside temperature
    if(insideTemp.updated)
    {
        insideTemp.updated = 0;
        tmp = insideTemp.value;
        if(insideTemp.changed())
        {

            if(tmp > 0x30)
            {
                disp_insideTempValue.print("HOT");
            }
            else
            {
                disp_insideTempValue.print(inTemp[tmp]);
            }

            if((tmp == 0) || tmp > 0x30)
            {
                disp_insideTempCaption->print("MAX");
            }
            else
            {
                disp_insideTempCaption->print("TEMP");
            }
        }
    }

    // blower speed
    if(blowerSpeed.updated && blowerSpeed.changed() && climateEnabled && (displayMode == DISP_MODE_NORMAL))
    {
        blowerSpeed.updated = 0;
        tmp = blowerSpeed.value >> 5;
        for(BYTE i = 0; i < 7; i++)
        {
            if(tmp > i)
            {
                disp_scaleValue[i].show();
            }
            else
            {
                disp_scaleValue[i].hide();
            }
        }
    }

    // blowing mode
    if(blowMode.updated)
    {
        blowMode.updated = 0;
        wtmp = blowMode.value;
        if(wtmp)
        {
            disp_body.show();
        }
        if(blowMode.changed())
        {
            // auto
            if(wtmp & 0x8000)
            {
                disp_auto.show();
            }
            else
            {
                disp_auto.hide();
            }

            // windshield
            if((wtmp & 0x1000) || (wtmp & 0x0040))
            {
                disp_windShield.show();
            }
            else
            {
                disp_windShield.hide();
            }

            // feet
            if((wtmp & 0x0100) || (wtmp & 0x0020) || (wtmp & 0x0040))
            {
                disp_feetArrow.show();
            }
            else
            {
                disp_feetArrow.hide();
            }

            // head
            if((wtmp & 0x0100) || (wtmp & 0x0080))
            {
                disp_headArrow.show();
            }
            else
            {
                disp_headArrow.hide();
            }
        }
    }
}
//-----------------------------------------------------------------------------
void tuneClimate()
{
    // включаем кондиционер, если целевая температура ниже наружной температуры

    // периодически мигаем кнопкой кондиционера, если целевая температура выше наружной температуры
    // одновременно меняем цвет наружной температуры на голубой
}
//-----------------------------------------------------------------------------
