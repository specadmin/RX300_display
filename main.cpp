//-----------------------------------------------------------------------------
#include <avr/pgmspace.h>
#include "avr-misc/avr-misc.h"
#include "options.h"
#include "avr-twi/twi.h"
#include "DS3231/ds3231.h"
#include "avr-debug/debug.h"
#include "toyota-mpx/mpx.h"
#include "RTD2660AVR/display.h"
#include "improvedOSD.h"
#include "main.h"
#include "data.h"

#include "body.h"
#include "clock.h"
#include "climate.h"
#include "global.h"
#include "media.h"
#include "tripComp.h"
#include "mpx-data.h"
//-----------------------------------------------------------------------------
void configureClassicDisplay(CFontMap* map);
BYTE FPS = 0;
//-----------------------------------------------------------------------------
int main()
{
    DDRB = 0x0F; PORTB = 0x0F;

    DEBUG_INIT();



    twi_init();
    struct DisplayConfig config;
    config.backgroundColor = 0x000000;
    display.init(config);
    OSD.setOrigin(0,0);
    OSD.uploadColorPallete((BYTE*)pallete);

    //display.showVideo(VS_AV2);
    display.on();

    CFontMap* map1 = &OSD.maps[0];
    configureClassicDisplay(map1);

    hideCar();
    //onIgnitionOff();

    mpx_init(mpxReceiver);

    //display.hideBackground();
    OSD.show(map1);

    options.showFPS = 0;


    //disp_mediaText3.print(" 102.7");
//
////    disp_buttons[0]->print("< DISC");
////    disp_buttons[1]->print("DISC >");
//    disp_buttons[0]->print(DOWN, FONT_GRAPHICS);
//    disp_buttons[1]->print(UP, FONT_GRAPHICS);
//
//    disp_buttons[2]->print(BKWD2, FONT_GRAPHICS);
//    disp_buttons[3]->print(FWD2, FONT_GRAPHICS);
//
//    disp_buttons[4]->print("RPT");
//    disp_buttons[5]->print("RAND");
//    disp_buttons[6]->print("SCAN");
//
//    //disp_buttons[7]->hide();
//    disp_buttons[7]->print("PSCAN");

    //disp_auto.hide();
    //disp_headArrow.hide();
    //disp_windShield.hide();
    //memory_usage();


    //display.showVideo(VS_AV2);
    //delay(4);
    //display.hideVideo();
    //OSD.setTransparency(3, BLEND_ALL);




    enable_interrupts();

    RTC_init();

    showCar();

    while(1)
    {
        displayClock();
        displayClimate();
        calculateAverageSpeed();
        displayTripInfo();
        tuneClimate();
        FPS++;
    }

    return 0;
}
//-----------------------------------------------------------------------------
void configureClassicDisplay(CFontMap* map)
{
    struct FontRowStyle rowStyle;
    struct FontAreaStyle fontStyle;

    WindowConfig wndConf;
    wndConf.left = X_OFFSET;
    wndConf.top = Y_OFFSET;
    wndConf.width = 750;
    wndConf.height = 460;
    wndConf.bodyColor = CL_RX300_BACKGROUND;
    wndConf.borderWidth = 0;
    OSD.window[0].configure(wndConf);

    wndConf.top = Y_OFFSET + 124;
    wndConf.height = 16;
    wndConf.bodyColor = CL_BLACK;
    OSD.window[1].configure(wndConf);

    wndConf.top = Y_OFFSET + 286;
    wndConf.height = 6;
    OSD.window[2].configure(wndConf);

    // SECTION 1
    map->addEmptyRow(Y_OFFSET + 1);
    CFontRow* line0top = map->addRow(12, rowStyle);

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.fontColor = CL_BLACK;
    line0top->addArea(X_OFFSET - 10, fontStyle, 1, MG_RD25pxTL);

    fontStyle.visible = false;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    disp_tripComp.add(line0top->addArea(0, fontStyle, 23, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));
    disp_outsideTemp.add(line0top->addArea(X_OFFSET + 293, fontStyle, 15, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));
    disp_alarm.add(line0top->addArea(X_OFFSET + 489, fontStyle, 20, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

    fontStyle.visible = true;
    fontStyle.fontColor = CL_BLACK;
    line0top->addArea(X_OFFSET + 728, fontStyle, 1, MG_RD25pxTR);

    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.fontColor = CL_RX300_BACKGROUND;
    fontStyle.visible = false;
    fontStyle.bgColor = CL_RX300_FOREGROUND;
    fontStyle.fontFace = FONT_BASIC;
    rowStyle.tracking = 2;
    CFontRow* titles1 = map->addRow(18, rowStyle);

    fontStyle.tracking = 3;
    disp_tripComp.add(titles1->addArea(X_OFFSET + 2, 276, "CRUISE INFORMATION", fontStyle));

    fontStyle.tracking = 0;
    disp_outsideTemp.add(titles1->addArea(X_OFFSET + 293, 180, "OUTSIDE TEMP", fontStyle));

    disp_alarm.add(titles1->addArea(X_OFFSET + 489, 240, "ALARM", fontStyle));

    rowStyle.tracking = 0;
    CFontRow* line0bot = map->addRow(4, rowStyle);

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_GRAPHICS;
    disp_tripComp.add(line0bot->addArea(X_OFFSET + 2, fontStyle, 23, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));
    disp_outsideTemp.add(line0bot->addArea(X_OFFSET + 293, fontStyle, 15, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));
    disp_alarm.add(line0bot->addArea(X_OFFSET + 489, fontStyle, 20, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));

    map->addEmptyRow(22);

    // trip computer
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_BASIC;
    rowStyle.tracking = 0;
    CFontRow* row12 = map->addRow(18, rowStyle);
    CFontRow* row13 = map->addRow(11, rowStyle);
    CFontRow* row14 = map->addRow(18, rowStyle);

    // trip computer caption
    //fontStyle.bgColor = CL_BLUE;
    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.tracking = 2;
    disp_tripCompCaption1 = row12->addArea(X_OFFSET + 2, 62, 5, fontStyle);
    disp_tripCompCaption2 = row14->addArea(X_OFFSET + 2, 62, 5, fontStyle);
    disp_tripComp.add(disp_tripCompCaption1);
    disp_tripComp.add(disp_tripCompCaption2);


    // trip computer value
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_tripCompValue.add(row12->addArea(X_OFFSET + 68, 120, 11, fontStyle));
    disp_tripCompValue.add(row13->addArea(X_OFFSET + 68, 120, 11, fontStyle));
    disp_tripCompValue.add(row14->addArea(X_OFFSET + 68, 120, 11, fontStyle));

    fontStyle.fontFace = FONT_NARROW;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 2;
    disp_tripCompUnit = row14->addArea(X_OFFSET + 195, 70, 7, fontStyle);
    disp_tripComp.add(disp_tripCompUnit);


    // outside temp value
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_outsideTempValue.add(row12->addArea(X_OFFSET + 315, 100, 9, fontStyle));
    disp_outsideTempValue.add(row13->addArea(X_OFFSET + 315, 100, 9, fontStyle));
    disp_outsideTempValue.add(row14->addArea(X_OFFSET + 315, 100, 9, fontStyle));


    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.alignment = ALIGN_LEFT;
    disp_outsideTemp.add(row12->addArea(0, 36, units.DEG_C, fontStyle));

    // clock
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_clockH.add(row12->addArea(_DISP_CLOCK_LEFT, 100, 6, fontStyle));
    disp_clockH.add(row13->addArea(_DISP_CLOCK_LEFT, 100, 6, fontStyle));
    disp_clockH.add(row14->addArea(_DISP_CLOCK_LEFT, 100, 6, fontStyle));
    disp_clockH.print("00");

    // clock colon
    fontStyle.blinkMode = BLINK_ALL;
    disp_clockColon.add(row12->addArea(_DISP_CLOCK_LEFT + 106, fontStyle, 1, _topDot));
    disp_clockColon.add(row14->addArea(_DISP_CLOCK_LEFT + 106, fontStyle, 1, _botDot));

    fontStyle.blinkMode = BLINK_NONE;
    disp_clockM.add(row12->addArea(_DISP_CLOCK_LEFT + 124, 100, 6, fontStyle));
    disp_clockM.add(row13->addArea(_DISP_CLOCK_LEFT + 124, 100, 6, fontStyle));
    disp_clockM.add(row14->addArea(_DISP_CLOCK_LEFT + 124, 100, 6, fontStyle));
    disp_clockM.print("00");

    fontStyle.visible = true;
    map->addEmptyRow(9);
    rowStyle.tracking = 0;
    fontStyle.tracking = 0;
    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_BLACK;
    fontStyle.fontFace = FONT_GRAPHICS;
    CFontRow* line1top = map->addRow(12, rowStyle);
    line1top->addArea(X_OFFSET - 10, fontStyle, 1,  MG_RD25pxBL);
    line1top->addArea(X_OFFSET + 728, fontStyle, 1, MG_RD25pxBR);


    map->addEmptyRow(16);
    CFontRow* line1bot = map->addRow(12, rowStyle);
    line1bot->addArea(X_OFFSET - 10, fontStyle, 1, MG_RD25pxTL);

    fontStyle.visible = false;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    disp_brightness.add(line1bot->addArea(_DISP_SCALE_LEFT - 2, fontStyle, 11, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

    fontStyle.visible = true;
    fontStyle.fontColor = CL_BLACK;
    line1bot->addArea(X_OFFSET + 728, fontStyle, 1, MG_RD25pxTR);

    // section 2
    CFontRow* row20b = map->addRow(18, rowStyle);
    CFontRow* row20c = map->addRow(4, rowStyle);

    // fuel remains
    fontStyle.visible = false;
    fontStyle.fontColor = CL_YELLOW;
    disp_fuel.add(row20b->addArea(_DISP_CAR_LEFT - 21, fontStyle, 2, MG_FUEL_2, MG_FUEL_1));
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 3;
    disp_fuel_value = row20b->addArea(_DISP_CAR_LEFT, 36, "54", fontStyle);
    disp_fuel.add(disp_fuel_value);
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.fontFace = FONT_NARROW;
    disp_fuel.add(row20b->addArea(_DISP_CAR_LEFT + 40, 30, KM, fontStyle));
//    disp_fuel.hide();

    // brightness
    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.fontColor = CL_RX300_BACKGROUND;
    fontStyle.bgColor = CL_RX300_FOREGROUND;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 2;
    disp_brightness.add(row20b->addArea(_DISP_SCALE_LEFT - 2, 132, "BRIGHTNESS", fontStyle));
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.tracking = 0;
    disp_brightness.add(row20c->addArea(_DISP_SCALE_LEFT - 2, fontStyle, 11,  MG_FILL, MG_RD8pxBL, MG_RD8pxBR));

    CFontRow* row20 = map->addRow(16, rowStyle);
    CFontRow* row21 = map->addRow(18, rowStyle);
    CFontRow* row22 = map->addRow(8, rowStyle);
    CFontRow* row23 = map->addRow(18, rowStyle);
    CFontRow* row24 = map->addRow(11, rowStyle);
    CFontRow* row25 = map->addRow(18, rowStyle);
    CFontRow* row26 = map->addRow(7, rowStyle);


    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;

    // rear left tire pressure
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 3;
    fontStyle.fontColor = CL_YELLOW;
    disp_tire_value[REAR_LEFT] = row25->addArea(_DISP_CAR_LEFT - 59, 34, "1.0", fontStyle);
    disp_tire[REAR_LEFT].add(disp_tire_value[REAR_LEFT]);

    // front left tire pressure
    disp_tire_value[FRONT_LEFT] = row21->addArea(_DISP_CAR_LEFT - 59, 34, "1.5", fontStyle);
    disp_tire[FRONT_LEFT].add(disp_tire_value[FRONT_LEFT]);

    // left-side tires
    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.tracking = 0;
    disp_tire[FRONT_LEFT].add(row20->addArea(_DISP_CAR_LEFT - 12, fontStyle, 1, MG_FL_TIRE_1));
    disp_tire[FRONT_LEFT].add(row21->addArea(_DISP_CAR_LEFT - 21, fontStyle, 2, MG_FL_TIRE_2, MG_TIRE_LINE));
    disp_tire[REAR_LEFT].add(row25->addArea(_DISP_CAR_LEFT - 21, fontStyle, 2, MG_RL_TIRE, MG_TIRE_LINE));

    // car
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    disp_car.add(row25->addArea(_DISP_CAR_LEFT, fontStyle, 4, MG_CAR_BACK_2, MG_CAR_BACK_1, MG_CAR_BACK_3));

    // left-side doors
    fontStyle.fontColor = CL_YELLOW;
    disp_door_BL = row24->addArea(_DISP_CAR_LEFT - 10, fontStyle, 1, MG_CAR_RL_DOOR_OPEN_1);
    disp_door_FBL = row23->addArea(_DISP_CAR_LEFT - 10, fontStyle, 1, MG_CAR_L_DOORS_2_ALL_OPEN);
    disp_door_FL = row22->addArea(_DISP_CAR_LEFT - 10, fontStyle, 1, MG_CAR_FL_DOOR_OPEN_1);
    disp_doors.add(disp_door_FL);
    disp_doors.add(disp_door_BL);
    disp_doors.add(disp_door_FBL);

    fontStyle.fontColor = CL_RX300_FOREGROUND;
    disp_car.add(row24->addArea(_DISP_CAR_LEFT, fontStyle, 1, MG_CAR_L_DOORS_12));
    disp_car.add(row23->addArea(_DISP_CAR_LEFT, fontStyle, 1, MG_CAR_L_DOORS_22));
    disp_car.add(row22->addArea(_DISP_CAR_LEFT, fontStyle, 2, MG_CAR_L_FRONT_ROOF, MG_CAR_L_DOORS_32));
    disp_car.add(row21->addArea(_DISP_CAR_LEFT, fontStyle, 2, MG_CAR_L_SIDE_12, MG_CAR_L_SIDE_11));
    disp_car.add(row20->addArea(_DISP_CAR_LEFT, fontStyle, 4, MG_CAR_FRONT_CENTER, MG_CAR_L_SIDE_21, MG_CAR_R_SIDE_21));
    disp_car.add(row21->addArea(0, fontStyle, 2, MG_CAR_R_SIDE_11, MG_CAR_R_SIDE_12));
    disp_car.add(row22->addArea(0, fontStyle, 2, MG_CAR_R_DOORS_32, MG_CAR_R_FRONT_ROOF));
    disp_car.add(row23->addArea(_DISP_CAR_LEFT + 28, fontStyle, 1, MG_CAR_R_DOORS_22));
    disp_car.add(row24->addArea(_DISP_CAR_LEFT + 28, fontStyle, 1, MG_CAR_R_DOORS_12));

    // right-side doors
    fontStyle.fontColor = CL_YELLOW;
    disp_door_BR = row24->addArea(0, fontStyle, 1, MG_CAR_RR_DOOR_OPEN_1);
    disp_door_FBR = row23->addArea(0, fontStyle, 1, MG_CAR_R_DOORS_2_ALL_OPEN);
    disp_door_FR = row22->addArea(0, fontStyle, 1, MG_CAR_FR_DOOR_OPEN_1);
    disp_doors.add(disp_door_FR);
    disp_doors.add(disp_door_BR);
    disp_doors.add(disp_door_FBR);

    // right-side tires
    fontStyle.fontColor = CL_YELLOW;
    disp_tire[FRONT_RIGHT].add(row20->addArea(0, fontStyle, 1, MG_FR_TIRE_1));
    disp_tire[FRONT_RIGHT].add(row21->addArea(0, fontStyle, 2, MG_TIRE_LINE, MG_FR_TIRE_2));
    disp_tire[REAR_RIGHT].add(row25->addArea(0, fontStyle, 2, MG_TIRE_LINE, MG_RR_TIRE));

    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 3;

    // rear right tire pressure
    disp_tire_value[REAR_RIGHT] = row25->addArea(_DISP_CAR_LEFT + 66, 36, "0.6", fontStyle);
    disp_tire[REAR_RIGHT].add(disp_tire_value[REAR_RIGHT]);

    // front right tire pressure
    disp_tire_value[FRONT_RIGHT] = row21->addArea(_DISP_CAR_LEFT + 66, 36, "2.0", fontStyle);
    disp_tire[FRONT_RIGHT].add(disp_tire_value[FRONT_RIGHT]);


    // inside temp value
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    disp_insideTempCaption = row21->addArea(_DISP_TEMP_LEFT, 60, 5, fontStyle);
    disp_insideTemp.add(disp_insideTempCaption);

    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.tracking = 0;
    disp_insideTempValue.add(row23->addArea(_DISP_TEMP_LEFT, 140, 12, fontStyle));
    disp_insideTempValue.add(row24->addArea(_DISP_TEMP_LEFT, 140, 12, fontStyle));
    disp_insideTempValue.add(row25->addArea(_DISP_TEMP_LEFT, 140, 12, fontStyle));

    // fan
    fontStyle.fontFace = FONT_GRAPHICS;
    disp_fan.add(row20->addArea(_DISP_FAN_LEFT, fontStyle, 3, MG_FAN12, MG_FAN11, MG_FAN13));
    disp_fan.add(row21->addArea(_DISP_FAN_LEFT, fontStyle, 3, MG_FAN22, MG_FAN21, MG_FAN23));
    disp_fan.add(row22->addArea(_DISP_FAN_LEFT, fontStyle, 3, MG_FAN32, MG_FAN31, MG_FAN33));

    // scale
    disp_scaleHolder.add(row24->addArea(_DISP_SCALE_LEFT - 8, fontStyle, 1, MG_SHV_2));
    disp_scaleHolder.add(row25->addArea(_DISP_SCALE_LEFT - 8, fontStyle, 1, MG_SHV));
    BYTE i;
    WORD left;
    for(i = 0, left = _DISP_SCALE_LEFT; i < 7; i++, left += 19)
    {
        switch(i)
        {
        case 0:
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_TOP12, MG_SC_TOP11));
            break;
        case 1:
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_TOP22, MG_SC_TOP21));
            break;
        case 2:
            disp_scaleValue[i].add(row23->addArea(left, fontStyle, 2, MG_SC_TOP32, MG_SC_TOP31));
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 3:
            disp_scaleValue[i].add(row23->addArea(left, fontStyle, 2, MG_SC_TOP42, MG_SC_TOP41));
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 4:
            disp_scaleValue[i].add(row23->addArea(left, fontStyle, 2, MG_SC_TOP52, MG_SC_TOP51));
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 5:
            disp_scaleValue[i].add(row22->addArea(left, fontStyle, 2, MG_SC_TOP62, MG_SC_TOP61));
            disp_scaleValue[i].add(row23->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 6:
            disp_scaleValue[i].add(row21->addArea(left, fontStyle, 2, MG_SC_TOP72, MG_SC_TOP71));
            disp_scaleValue[i].add(row22->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            disp_scaleValue[i].add(row23->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            disp_scaleValue[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        }
        disp_scaleValue[i].add(row25->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
        disp_scale.add(disp_scaleValue[i]);
    }

    // scale right border
    disp_scaleHolder.add(row21->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV));
    disp_scaleHolder.add(row22->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV));
    disp_scaleHolder.add(row23->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV));
    disp_scaleHolder.add(row24->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV));
    disp_scaleHolder.add(row25->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV));

    // scale bottom border
    disp_scaleHolder.add(row26->addArea(_DISP_SCALE_LEFT - 8, fontStyle, 12, MG_SHB, MG_SHBL, MG_SHBR));
    disp_scale.add(disp_scaleHolder);

    // windshield heater
    disp_windShield.add(row21->addArea(_DISP_WSHIELD_LEFT, fontStyle, 2, MG_WSHIELD12, MG_WSHIELD11));
    disp_windShield.add(row21->addArea(_DISP_WSHIELD_LEFT + 24, fontStyle, 2, MG_WSHIELD14, MG_WSHIELD13));
    disp_windShield.add(row22->addArea(_DISP_WSHIELD_LEFT + 7, fontStyle, 3, MG_WSHIELD22, MG_WSHIELD21, MG_WSHIELD23));
    disp_windShield.add(row23->addArea(_DISP_WSHIELD_LEFT + 16, fontStyle, 2, MG_WSHIELD32, MG_WSHIELD31));
    disp_flowDirection.add(disp_windShield);

    // auto
    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_GRAPHICS;
    disp_auto.add(row24->addArea(_DISP_AUTO_LEFT, fontStyle, 6, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.fontColor = CL_RX300_BACKGROUND;
    fontStyle.bgColor = CL_RX300_FOREGROUND;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 2;
    disp_auto.add(row25->addArea(_DISP_AUTO_LEFT, 72, "AUTO", fontStyle));

    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;

    // pressure
//    row20b->addArea(630, 100, "1013 KPA",fontStyle);

    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.tracking = 0;
    disp_auto.add(row26->addArea(_DISP_AUTO_LEFT, fontStyle, 6,  MG_FILL, MG_RD8pxBL, MG_RD8pxBR));
    disp_flowDirection.add(disp_auto);


    // feet arrow
    disp_feetArrow.add(row22->addArea(_DISP_LEGS_ARROW_LEFT, fontStyle, 2, MG_V_ARROW12, MG_V_ARROW11));
    disp_feetArrow.add(row23->addArea(_DISP_LEGS_ARROW_LEFT, fontStyle, 2, MG_V_ARROW22, MG_V_ARROW21));
    disp_feetArrow.add(row24->addArea(_DISP_LEGS_ARROW_LEFT + 4, fontStyle, 2, MG_V_ARROW32, MG_V_ARROW31));
    disp_flowDirection.add(disp_feetArrow);

    // head arrow
    disp_headArrow.add(row21->addArea(_DISP_HEAD_ARROW_LEFT, fontStyle, 3, MG_H_ARROW_BODY, MG_H_ARROW_BODY, MG_H_ARROW_RT));
    disp_headArrow.add(row22->addArea(_DISP_HEAD_ARROW_LEFT + 14, fontStyle, 1, MG_H_ARROW_RB));
    disp_flowDirection.add(disp_headArrow);

    // head
    disp_body.add(row21->addArea(_DISP_BODY_LEFT + 56, fontStyle, 2, MG_ROUND_D17_R, MG_ROUND_D17_L));

    // body
    disp_body.add(row22->addArea(_DISP_BODY_LEFT + 50, fontStyle, 2, MG_BODY12, MG_BODY11));

    disp_body.add(row23->addArea(_DISP_BODY_LEFT + 15, fontStyle, 2, MG_BODY22, MG_BODY21));
    disp_body.add(row23->addArea(_DISP_BODY_LEFT + 39, fontStyle, 2, MG_BODY24, MG_BODY23));

    disp_body.add(row24->addArea(_DISP_BODY_LEFT + 9, fontStyle, 2, MG_BODY32, MG_BODY31));
    disp_body.add(row24->addArea(_DISP_BODY_LEFT + 31, fontStyle, 2, MG_BODY34, MG_BODY33));

    disp_body.add(row25->addArea(_DISP_BODY_LEFT, fontStyle, 2, MG_BODY42, MG_BODY41));
    disp_body.add(row25->addArea(_DISP_BODY_LEFT + 42, fontStyle, 1, MG_BODY43));

    disp_flowDirection.add(disp_body);

    // section 3
    map->addEmptyRow(54);
    CFontRow* row31 = map->addRow(18, rowStyle);
    CFontRow* row32 = map->addRow(11, rowStyle);
    CFontRow* row33 = map->addRow(18, rowStyle);
    map->addEmptyRow(17);
/*    CFontRow* row33b =*/ map->addRow(16, rowStyle);
    map->addEmptyRow(5);
    CFontRow* row34 = map->addRow(7, rowStyle);
    CFontRow* row35 = map->addRow(8, rowStyle);
    CFontRow* row36 = map->addRow(18, rowStyle);
    CFontRow* row37 = map->addRow(6, rowStyle);
    CFontRow* row38 = map->addRow(12, rowStyle);

    // media text 1
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    disp_mediaText1.add(row31->addArea(_DISP_MEDIA_TEXT_1_LEFT, 150, 12, fontStyle));
    disp_mediaText1.add(row32->addArea(_DISP_MEDIA_TEXT_1_LEFT, 150, 12, fontStyle));
    disp_mediaText1.add(row33->addArea(_DISP_MEDIA_TEXT_1_LEFT, 150, 12, fontStyle));

    // disc
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 2;
    row31->addArea(_DISP_MEDIA_TEXT_2_LEFT - 64, 50, "DISC", fontStyle);

    // channel label
    row33->addArea(_DISP_MEDIA_TEXT_2_LEFT - 64, 50, "CH", fontStyle);

    // media text 2
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 0;
    disp_mediaText2.add(row31->addArea(_DISP_MEDIA_TEXT_2_LEFT, 40, 3, fontStyle));
    disp_mediaText2.add(row32->addArea(_DISP_MEDIA_TEXT_2_LEFT, 40, 3, fontStyle));
    disp_mediaText2.add(row33->addArea(_DISP_MEDIA_TEXT_2_LEFT, 40, 3, fontStyle));

    // track
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 2;
    row31->addArea(_DISP_MEDIA_TEXT_3_LEFT - 80, 70, "TRACK", fontStyle);

    // mute
    row33->addArea(_DISP_MEDIA_TEXT_3_LEFT - 80, 70, "MUTE", fontStyle);

    // media text 3
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 0;
    disp_mediaText3.add(row31->addArea(_DISP_MEDIA_TEXT_3_LEFT, 240, 22, fontStyle));
    disp_mediaText3.add(row32->addArea(_DISP_MEDIA_TEXT_3_LEFT, 240, 22, fontStyle));
    disp_mediaText3.add(row33->addArea(_DISP_MEDIA_TEXT_3_LEFT, 240, 22, fontStyle));

    // ST
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 2;
    row31->addArea(X_OFFSET + 659, 30, "ST", fontStyle);

    // metal
    row33->addArea(X_OFFSET + 659, 70, "METAL", fontStyle);

    // media states
/*
    disp_RPT = row33b->addArea(_DISP_BUTTONS_LEFT + 2 * (_DISP_BUTTON_WIDTH + _DISP_BUTTON_DISTANCE), 65, "D.RPT", fontStyle);
    disp_SKIP = row33b->addArea(0, 52, "SKIP", fontStyle);
    disp_RAND = row33b->addArea(0, 68, "D.RAND", fontStyle);


    fontStyle.alignment = ALIGN_CENTER;
    disp_DSCAN = row33b->addArea(_DISP_BUTTONS_LEFT + 3 * (_DISP_BUTTON_WIDTH + _DISP_BUTTON_DISTANCE), 82, "D.SCAN", fontStyle);
    disp_PSCAN = row33b->addArea(0, 82, "P.SCAN", fontStyle);

*/

    // bottom left corner
    fontStyle.visible = true;
    fontStyle.fontColor = CL_BLACK;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.tracking = 0;
    row38->addArea(X_OFFSET - 10, fontStyle, 1,  MG_RD25pxBL);

    // buttons
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    for(i = 0; i < 1; i++)
    {
        left = _DISP_BUTTONS_LEFT + i * (_DISP_BUTTON_WIDTH + _DISP_BUTTON_DISTANCE);
        row34->addArea(left, fontStyle, 14, MG_SHT, MG_SHTL, MG_SHTR);
        row35->addArea(left, fontStyle, 1, MG_SHV);
        row36->addArea(left, fontStyle, 1, MG_SHV);
        row37->addArea(left, fontStyle, 1, MG_SHV);
        row38->addArea(left, fontStyle, 14, MG_SHB, MG_SHBL, MG_SHBR);
        fontStyle.fontFace = FONT_BASIC;
        fontStyle.alignment = ALIGN_CENTER;
        fontStyle.tracking = 2;
        disp_buttons[i * 2] = row36->addArea(0, 55, "<", fontStyle);
        row36->addArea(0, 50, "DISC", fontStyle);
        disp_buttons[i * 2 + 1] = row36->addArea(0, 55, ">", fontStyle);
        fontStyle.fontFace = FONT_GRAPHICS;
        fontStyle.tracking = 0;
        left += _DISP_BUTTON_WIDTH;
        row35->addArea(left, fontStyle, 1, MG_SHV);
        row36->addArea(left, fontStyle, 1, MG_SHV);
        row37->addArea(left, fontStyle, 1, MG_SHV);
    }

    for(i = 1; i < 4; i++)
    {
        left = _DISP_BUTTONS_LEFT + i * (_DISP_BUTTON_WIDTH + _DISP_BUTTON_DISTANCE);
        row34->addArea(left, fontStyle, 14, MG_SHT, MG_SHTL, MG_SHTR);
        row35->addArea(left, fontStyle, 1, MG_SHV);
        row36->addArea(left, fontStyle, 1, MG_SHV);
        row37->addArea(left, fontStyle, 1, MG_SHV);
        row38->addArea(left, fontStyle, 14, MG_SHB, MG_SHBL, MG_SHBR);
        fontStyle.fontFace = FONT_BASIC;
        fontStyle.alignment = ALIGN_CENTER;
        fontStyle.tracking = 2;
        disp_buttons[i * 2] = row36->addArea(0, _DISP_BUTTON_WIDTH / 2 - 2, " ", fontStyle);
        disp_buttons[i * 2 + 1] = row36->addArea(0, _DISP_BUTTON_WIDTH / 2 - 2, " ", fontStyle);
        fontStyle.fontFace = FONT_GRAPHICS;
        fontStyle.tracking = 0;
        left += _DISP_BUTTON_WIDTH;
        row35->addArea(left, fontStyle, 1, MG_SHV);
        row36->addArea(left, fontStyle, 1, MG_SHV);
        row37->addArea(left, fontStyle, 1, MG_SHV);
    }
    // bottom right corner
    fontStyle.fontColor = CL_BLACK;
    fontStyle.fontFace = FONT_GRAPHICS;
    row38->addArea(X_OFFSET + 728, fontStyle, 1, MG_RD25pxBR);
}
//-----------------------------------------------------------------------------
