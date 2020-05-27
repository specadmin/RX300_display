//-----------------------------------------------------------------------------
#include <stdio.h>
#include <avr/pgmspace.h>
#include "avr-misc/avr-misc.h"
#include "avr-debug/debug.h"
#include "toyota-mpx/mpx.h"
#include "RTD2660AVR/display.h"
#include "improvedOSD.h"
#include "main.h"
//-----------------------------------------------------------------------------
BYTE mpx_buf[10] = { 0x00 };
void configureClassicDisplay(CFontMap* map);
void dispatchMPX(BYTE* data);
//-----------------------------------------------------------------------------
void onIgnitionOff()
{
    disp_tripComp.hide();
    disp_tripCompValue.hide();

    disp_brightness.hide();

    disp_outsideTemp.hide();
    disp_outsideTempValue.hide();

    disp_insideTemp.hide();
    disp_insideTempValue.hide();

    disp_fuel.hide();
    disp_fuel_value->hide();
    disp_fan.hide();
    disp_scale.hide();

    disp_flowDirection.hide();

    disp_media_set.hide();
}
//-----------------------------------------------------------------------------
void hideCar()
{
    disp_car.hide();
    for(BYTE i = 0; i < 4; i++)
    {
        disp_tire[i].hide();
        disp_doors.hide();
    }
}
//-----------------------------------------------------------------------------
#pragma GCC diagnostic ignored "-Wunused-parameter"
void mpxReceiver(BYTE size, const BYTE* buf)
{
    memcpy(mpx_buf, buf, size);
}
//-----------------------------------------------------------------------------
__inline void displayInsideTemp(BYTE data)
{
    static BYTE prevData = 0xFF;
    //DHEX8(1, data);
    if(data == prevData)
    {
        return;
    }

    //DHEX(data);
    static const char inTemp[][5] = {   "COLD", "16.5", "17.0", "17.5", "18.0", "18.5", "19.0", "19.5", "20.0", "20.5", "21.0",
                                        "21.5", "22.0", "22.5", "23.0", "23.5", "24.0", "24.5", "25.0", "25.5", "26.0", "26.5",
                                        "27.0", "27.5", "28.0", "28.5", "29.0", "29.5", "30.0", "30.5", "31.0", "31.5", "32.0",
                                        "32.5", "33.0", "33.5", "34.0", "34.5", "35.0", "35.5", "36.0", "36.5", "37.0", "37.5",
                                        "38.0", "38.5", "39.0" };
    if(mpx_buf[3] > 0x37)
    {
        disp_insideTemp.hide();
        disp_insideTempValue.hide();
        disp_auto.hide();
    }
    else
    {
        //disp_insideTemp.show();
        //disp_insideTempValue.show();
        if(data > 0x30)
        {
            disp_insideTempValue.print("HOT");
        }
        else
        {
            disp_insideTempValue.print(inTemp[data]);
        }

        if((data == 0) || data > 0x30)
        {
            disp_insideTempCaption->print("MAX");
        }
        else
        {
            disp_insideTempCaption->print("TEMP");
        }
    }
    prevData = data;
}
//-----------------------------------------------------------------------------
__inline void displayBlowMode(WORD data)
{
    static BYTE prevData = 0xFF;
    if(data == prevData)
    {
        return;
    }
    prevData = data;
    if(data)
    {
        disp_body.show();
        disp_scaleHolder.show();
    }

    // auto
    if(data & 0x8000)
    {
        disp_auto.show();
    }
    else
    {
        disp_auto.hide();
    }

    // windshield
    if((data & 0x1000) || (data & 0x0040))
    {
        disp_windShield.show();
    }
    else
    {
        disp_windShield.hide();
    }

    // feet
    if((data & 0x0100) || (data & 0x0020) || (data & 0x0040))
    {
        disp_feetArrow.show();
    }
    else
    {
        disp_feetArrow.hide();
    }

    // head
    if((data & 0x0100) || (data & 0x0080))
    {
        disp_headArrow.show();
    }
    else
    {
        disp_headArrow.hide();
    }
}
//-----------------------------------------------------------------------------
__inline void displayBlowerSpeed(BYTE data)
{
    static BYTE prevData = 0xFF;
    if(data == prevData)
    {
        return;
    }
    prevData = data;
    //DHEX(data);
    data >>= 5;
    if(data > 0)
    {
        disp_scaleHolder.show();
    }
    for(BYTE i = 0; i < 7; i++)
    {
        if(data > i)
        {
            disp_scaleValue[i].show();
        }
        else
        {
            disp_scaleValue[i].hide();
        }
    }
}
//-----------------------------------------------------------------------------
void dispatchMPX()
{
//    static char str[10];
    if(mpx_buf[0])
    {
        set_bit(PORTC, 7);
        switch(mpx_buf[2])
        {
//        case 0x24:  // vehicle status
//            if((mpx_buf[4] & 0x80) == 0)
//            {
//                onIgnitionOff();
//            }
//            break;
        case 0xCD:  // outside temperature
            //disp_outsideTemp.show();
            //disp_outsideTempValue.show();
            break;
            DHEX(mpx_buf[3]);
            //sprintf(str, "%d", (char) mpx_buf[3]);
            //disp_outsideTempValue.print(str);
            break;
        case 0x1B:  // climate: blower speed
            displayBlowerSpeed(mpx_buf[3]);
            break;
        case 0xFC:  // climate: blow mode
            //displayBlowMode(mpx_buf[3] << 8 | mpx_buf[4]);
            break;
        case 0xFD:  // climate: target inside temperature
            displayInsideTemp(mpx_buf[3]);
            break;
        }
        mpx_buf[0] = 0;
        clr_bit(PORTC, 7);
    }
}
//-----------------------------------------------------------------------------
int main()
{
    DEBUG_INIT();
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
    //halt();
    disp_tripCompValue.print("17.4");
    disp_outsideTempValue.print("24");

//    disp_insideTemp.print("HOT");
    //disp_AMPM.hide();
    //disp_alarm.hide();
    disp_clockH.print("23");
    disp_clockM.print("57");

    disp_tripCompUnit->print(L100KM);



    //disp_brightness.hide();

//    disp_scale[5].hide();
//    disp_scale[6].hide();

//    delay(1);
//
//    delay(1);
//
//    //OSD.hide();
//    AMPM->print("PM");

//    delay(2);
//
//    alarm.blink();
//    delay(4);
//    alarm.show();
//
//    delay(2);
////    AMPM->print("PM");
//    alarm.hide();
//    delay(4);
////    AMPM->print("");
//    alarm.show();
//
//    delay(4);
//
////    alarm.show();
    disp_mediaText1.print("FM1");
    disp_mediaText2.print("4");
    disp_mediaText3.print("26 13'48");
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
    memory_usage();


    //display.showVideo(VS_AV2);
    //delay(4);
    //display.hideVideo();
    //OSD.setTransparency(3, BLEND_ALL);



    //enable_interrupts();



//    BYTE* data;
//    BYTE dataSize;
    set_bit(DDRC, 6);
    set_bit(DDRC, 7);

    disp_insideTemp.show();
    disp_insideTempValue.show();

    while(1)
    {
        set_bit(PORTC, 6);
        disp_insideTempValue.print("23.4");
        clr_bit(PORTC, 6);
        mdelay(4);
    }
    while(1)
    {
        nop();
        dispatchMPX();
//        switch(readMessage(&data, &dataSize))
//        {
//        case MSG_MPX_DATA:
//            dispatchMPX(data);
//            break;
//        }
//        commitMessage();
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
    disp_outsideTemp.add(line0top->addArea(X_OFFSET + 293, fontStyle, 14, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));
    disp_alarm.add(line0top->addArea(X_OFFSET + 477, fontStyle, 21, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

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
    disp_outsideTemp.add(titles1->addArea(X_OFFSET + 293, 168, "OUTSIDE TEMP", fontStyle));

    disp_alarm.add(titles1->addArea(X_OFFSET + 477, 252, "ALARM", fontStyle));

    rowStyle.tracking = 0;
    CFontRow* line0bot = map->addRow(4, rowStyle);

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_GRAPHICS;
    disp_tripComp.add(line0bot->addArea(X_OFFSET + 2, fontStyle, 23, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));
    disp_outsideTemp.add(line0bot->addArea(X_OFFSET + 293, fontStyle, 14, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));
    disp_alarm.add(line0bot->addArea(X_OFFSET + 477, fontStyle, 21, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));

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
    fontStyle.tracking = 2;
    disp_tripCompCaption = row12->addArea(X_OFFSET + 2, 50, "AVG", fontStyle);
    disp_tripComp.add(disp_tripCompCaption);


    // trip computer value
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_tripCompValue.add(row12->addArea(X_OFFSET + 70, 120, 10, fontStyle));
    disp_tripCompValue.add(row13->addArea(X_OFFSET + 70, 120, 10, fontStyle));
    disp_tripCompValue.add(row14->addArea(X_OFFSET + 70, 120, 10, fontStyle));

    fontStyle.fontFace = FONT_NARROW;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 2;
    disp_tripCompUnit = row14->addArea(X_OFFSET + 195, 70, 7, fontStyle);
    disp_tripComp.add(disp_tripCompUnit);


    // outside temp value
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_outsideTempValue.add(row12->addArea(X_OFFSET + 325, 100, 9, fontStyle));
    disp_outsideTempValue.add(row13->addArea(X_OFFSET + 325, 100, 9, fontStyle));
    disp_outsideTempValue.add(row14->addArea(X_OFFSET + 325, 100, 9, fontStyle));


    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.alignment = ALIGN_LEFT;
    disp_outsideTemp.add(row12->addArea(0, 36, DEG_C, fontStyle));

    // clock
    fontStyle.visible = true;
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_clockH.add(row12->addArea(_DISP_CLOCK_LEFT, 100, 6, fontStyle));
    disp_clockH.add(row13->addArea(_DISP_CLOCK_LEFT, 100, 6, fontStyle));
    disp_clockH.add(row14->addArea(_DISP_CLOCK_LEFT, 100, 6, fontStyle));

    // clock colon
    fontStyle.blinkMode = BLINK_ALL;
    row12->addArea(_DISP_CLOCK_LEFT + 106, fontStyle, 1, _topDot);
    row14->addArea(_DISP_CLOCK_LEFT + 106, fontStyle, 1, _botDot);


    fontStyle.blinkMode = BLINK_NONE;
    disp_clockM.add(row12->addArea(_DISP_CLOCK_LEFT + 124, 100, 6, fontStyle));
    disp_clockM.add(row13->addArea(_DISP_CLOCK_LEFT + 124, 100, 6, fontStyle));
    disp_clockM.add(row14->addArea(_DISP_CLOCK_LEFT + 124, 100, 6, fontStyle));

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
