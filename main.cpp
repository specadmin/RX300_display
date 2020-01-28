//-----------------------------------------------------------------------------
#include <avr/pgmspace.h>
#include "avr-misc/avr-misc.h"
#include "avr-debug/debug.h"
#include "RTD2660AVR/display.h"
#include "improvedOSD.h"
#pragma GCC diagnostic ignored "-Wunused-variable"
//-----------------------------------------------------------------------------
enum OSD_pallete
{
    CL_TRANSPARENT = 0,
    CL_BLACK,
    CL_WHITE,
    CL_RED,
    CL_GREEN,
    CL_YELLOW,
    CL_BLUE,
    CL_BUTTON_FACE,
    CL_BUTTON_LIGHT,
    CL_BUTTON_SHADOW,
    CL_WINDOW,
    CL_WINDOW_BORDER,
    CL_WINDOW_SHADOW,
    CL_RX300_FOREGROUND,
    CL_RX300_BACKGROUND,
    CL_SIZE
};
//-----------------------------------------------------------------------------
enum
{
    MG_BLANK = 1,
    MG_FILL,
    MG_DEGREE,
    MG_C,
    MG_F,
    MG_6PXSKIP,
    MG_RD25pxBL,
    MG_RD25pxBR,
    MG_RD25pxTL,
    MG_RD25pxTR,
    MG_SHT,
    MG_SHTL,
    MG_SHTR,
    MG_SHB,
    MG_SHBL,
    MG_SHBR,
    MG_SHV,
    MG_SC_FILL1, MG_SC_FILL2,
    MG_SC_TOP11, MG_SC_TOP12,
    MG_SC_TOP21, MG_SC_TOP22,
    MG_SC_TOP31, MG_SC_TOP32,
    MG_SC_TOP41, MG_SC_TOP42,
    MG_SC_TOP51, MG_SC_TOP52,
    MG_SC_TOP61, MG_SC_TOP62,
    MG_SC_TOP71, MG_SC_TOP72,
    MG_RD8pxBL,
    MG_RD8pxBR,
    MG_RD8pxTL,
    MG_RD8pxTR,
    MG_FAN11, MG_FAN12, MG_FAN13,
    MG_FAN21, MG_FAN22, MG_FAN23,
    MG_FAN31, MG_FAN32, MG_FAN33,
    MG_H_ARROW_BODY,
    MG_H_ARROW_RT, MG_H_ARROW_RB,
    MG_V_ARROW11, MG_V_ARROW12,
    MG_V_ARROW21, MG_V_ARROW22,
    MG_V_ARROW31, MG_V_ARROW32,
    MG_ROUND_D17_L,MG_ROUND_D17_R,
    MG_BODY11, MG_BODY12,
    MG_BODY21, MG_BODY22, MG_BODY23, MG_BODY24,
    MG_BODY31, MG_BODY32, MG_BODY33, MG_BODY34,
    MG_BODY41, MG_BODY42, MG_BODY43,
    MG_WSHIELD11, MG_WSHIELD12, MG_WSHIELD13, MG_WSHIELD14,
    MG_WSHIELD21, MG_WSHIELD22, MG_WSHIELD23,
    MG_WSHIELD31, MG_WSHIELD32,
};
//-----------------------------------------------------------------------------
enum
{
    FONT_BASIC,
    FONT_GRAPHICS,
    FONT_NARROW,
    FONT_CRYSTAL
};
//-----------------------------------------------------------------------------
#define DEG_C   "\003\004"
#define KMPH    "\004\005\006\007"
#define L100KM  "\003\006\002\001\001\004\005"
//-----------------------------------------------------------------------------
const BYTE PROGMEM pallete[CL_SIZE][3]=
{
    {0, 120, 60},       // CL_TRANSPARENT
    {0, 0, 0},          // CL_BLACK
    {255, 255, 255},    // CL_WHITE
    {255, 0, 0},        // CL_RED
    {0, 255, 0},        // CL_GREEN
    {255, 255, 0},      // CL_YELLOW
    {0, 0, 255},        // CL_BLUE
    {100, 50, 10},      // CL_BUTTON_FACE
    {150, 70, 20},      // CL_BUTTON_LIGHT
    {50, 25, 5},        // CL_BUTTON_SHADOW
    {80, 80, 70},       // CL_WINDOW
    {200, 200, 180},    // CL_WINDOW_BORDER
    {20, 20, 10},       // CL_WINDOW_SHADOW
    {180, 180, 180},    // CL_RX300_FOREGROUND
    {25, 45, 65},       // CL_RX300_BACKGROUND
};
//-----------------------------------------------------------------------------
#define _DISP_TEMP_LEFT         210
#define _DISP_FAN_LEFT          360
#define _DISP_SCALE_LEFT        358
#define _DISP_AUTO_LEFT         537
#define _DISP_WSHIELD_LEFT      560
#define _DISP_BODY_LEFT         640
#define _DISP_HEAD_ARROW_LEFT   _DISP_BODY_LEFT + 14
#define _DISP_LEGS_ARROW_LEFT   _DISP_BODY_LEFT - 18
//-----------------------------------------------------------------------------
CFontAreaSet        disp_alarm;
CFontAreaSet        disp_tripCompAVG;
CImprovedAreaSet    disp_tripCompValue;
CImprovedAreaSet    disp_tripCompUnit;
CImprovedAreaSet    disp_outsideTemp;
CFontAreaSet        disp_AMPM;
CImprovedAreaSet    disp_clockH;
CImprovedAreaSet    disp_clockM;
CImprovedAreaSet    disp_insideTemp;
CFontAreaSet        disp_brightness;
CFontAreaSet        disp_fan;
CFontAreaSet        disp_scale[7];
CFontAreaSet        disp_windShield;
CFontAreaSet        disp_auto;
CFontAreaSet        disp_headArrow;
CFontAreaSet        disp_feetArrow;
CImprovedAreaSet    disp_mediaText1;
CImprovedAreaSet    disp_mediaText2;
CImprovedAreaSet    disp_mediaText3;
CFontArea*          disp_button[8];
//-----------------------------------------------------------------------------
void configureClassicDisplayMap(CFontMap* map);
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



    //OSD.setTransparency(3, BLEND_ALL);


    CFontMap* map1 = &OSD.maps[0];
    CFontMap* map2 = &OSD.maps[1];

    configureClassicDisplayMap(map1);

/*
    OSD.window[0]=createWindowEx(0, 0, 800, 240, CL_BLUE, CL_RED, 0, OSD_GRADIENT_CYAN_MORE | OSD_GRADIENT_VERTICAL, 4);        // sky
//    OSD.window[1]=createWindow(0, 0, 30, 100, OSD_WS_BORDER);
    OSD.window[4]=createWindowEx(0, 240, 800, 240, CL_BLACK, CL_RED, 0, OSD_GRADIENT_WHITE_MORE | OSD_GRADIENT_VERTICAL, 1);   // ground
    OSD.window[5]=createWindow(252, 140, 300, 180, OSD_WS_BORDER);
    //OSD.window[6]=createWindowEx(252, 140, 300, 180, CL_TRANSPARENT, CL_WHITE, 2);                                              // transparent window
    //OSD.window[6]=createWindowEx(250, 140, 300, 180, CL_BLACK, CL_WHITE, 2, OSD_GRADIENT_BLUE_MORE, 3);                  // gradient window
    OSD.window[6]=createWindow(280, 260, 100, 40, OSD_WS_BUTTON_1);                                                             // btnOK
    OSD.window[7]=createWindow(424, 260, 100, 40, OSD_WS_BUTTON_1);                                                             // btnCancel
    OSD.window[0].show();

    map1->addEmptyRow(181);

    rowStyle.doubleSize = true;
    rowStyle.tracking = 5;
    CFontRow* row1 = map1->addRow(36, rowStyle);

//    fontStyle.underlined = true;
    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.fontColor = CL_YELLOW;
//    fontStyle.bgColor = CL_RX300_FOREGROUND;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.blinkMode = BLINK_ALL;

    row1->addArea(251, 287, "HELLO WORLD!", fontStyle);

    map1->addEmptyRow(57);

    rowStyle.doubleSize = false;
    rowStyle.tracking = 3;
    fontStyle.blinkMode = BLINK_NONE;
    CFontRow* rowButtons = map1->addRow(18, rowStyle);

    fontStyle.fontColor = CL_WHITE;
    fontStyle.bgColor = CL_TRANSPARENT;
    rowButtons->addArea(251, 143, "OK", fontStyle);
    rowButtons->addArea(0, 143, "EXIT", fontStyle);

*/

//====================================================

    WindowConfig wndConf;
    wndConf.left = 25;
    wndConf.top = 10;
    wndConf.width = 750;
    wndConf.height = 460;
    wndConf.bodyColor = CL_RX300_BACKGROUND;
    wndConf.borderWidth = 0;
    OSD.window[0].configure(wndConf);

    wndConf.top = 134;
    wndConf.height = 16;
    wndConf.bodyColor = CL_BLACK;
    OSD.window[1].configure(wndConf);

    wndConf.top = 296;
    wndConf.height = 6;
    OSD.window[2].configure(wndConf);

//wndConf.left = 85;
//wndConf.top = 0;
//wndConf.width = 130;
//wndConf.height = 480;
//OSD.window[3].configure(wndConf);
//
//wndConf.left = 360;
//OSD.window[4].configure(wndConf);





    //alarm.hide();
    //display.hideBackground();
    OSD.show(map1);
    //halt();
    disp_tripCompValue.print("17.4");
    disp_outsideTemp.print("-34");
    disp_insideTemp.print("26.5");
    disp_AMPM.hide();
    disp_alarm.hide();
    disp_clockH.print("23");
    disp_clockM.print("57");

    disp_tripCompUnit.print(L100KM);

    //disp_brightness.hide();

//    disp_scale[5].hide();
//    disp_scale[6].hide();

    while(0)
    {
        disp_outsideTemp.print("24");
        mdelay(100);
        disp_outsideTemp.print("-24");
        mdelay(100);
    }

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
    disp_mediaText1.print("FESC");
    disp_mediaText2.print("2");
    disp_mediaText3.print("2345678");
    //disp_auto.hide();
    //disp_headArrow.hide();
    //disp_windShield.hide();
    memory_usage();


    //display.showVideo(VS_AV2);
    //delay(4);
    //display.hideVideo();
    //OSD.setTransparency(3, BLEND_ALL);

    delay(2);




    return 0;
}
//-----------------------------------------------------------------------------
void configureClassicDisplayMap(CFontMap* map)
{
    struct FontRowStyle rowStyle;
    struct FontAreaStyle fontStyle;

    // SECTION 1
    map->addEmptyRow(11);
    CFontRow* line0top = map->addRow(12, rowStyle);

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.fontColor = CL_BLACK;
    line0top->addArea(15, fontStyle, 1, MG_RD25pxTL);

    fontStyle.fontColor = CL_RX300_FOREGROUND;
    line0top->addArea(0, fontStyle, 23, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR);
    line0top->addArea(318, fontStyle, 14, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR);
    disp_alarm.add(line0top->addArea(502, fontStyle, 21, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

    fontStyle.fontColor = CL_BLACK;
    line0top->addArea(753, fontStyle, 1, MG_RD25pxTR);

    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.fontColor = CL_RX300_BACKGROUND;
    fontStyle.bgColor = CL_RX300_FOREGROUND;
    fontStyle.fontFace = FONT_BASIC;
    rowStyle.tracking = 2;
    CFontRow* titles1 = map->addRow(18, rowStyle);

    fontStyle.tracking = 3;
    CFontArea* cruise = titles1->addArea(27, 276, "CRUISE INFORMATION", fontStyle);

    fontStyle.tracking = 0;
    titles1->addArea(318, 168, "OUTSIDE TEMP", fontStyle);

    disp_alarm.add(titles1->addArea(502, 252, "ALARM", fontStyle));

    rowStyle.tracking = 0;
    CFontRow* line0bot = map->addRow(4, rowStyle);

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_GRAPHICS;
    line0bot->addArea(27, fontStyle, 23, MG_FILL, MG_RD8pxBL, MG_RD8pxBR);
    line0bot->addArea(318, fontStyle, 14, MG_FILL, MG_RD8pxBL, MG_RD8pxBR);
    disp_alarm.add(line0bot->addArea(502, fontStyle, 21, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));

    map->addEmptyRow(22);

    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_BASIC;
    rowStyle.tracking = 0;
    CFontRow* row12 = map->addRow(18, rowStyle);
    CFontRow* row13 = map->addRow(11, rowStyle);
    CFontRow* row14 = map->addRow(18, rowStyle);

    fontStyle.tracking = 2;
    disp_tripCompAVG.add(row12->addArea(27, 50, "AVG", fontStyle));





    // trip computer
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_tripCompValue.add(row12->addArea(95, 120, 10, fontStyle));
    disp_tripCompValue.add(row13->addArea(95, 120, 10, fontStyle));
    disp_tripCompValue.add(row14->addArea(95, 120, 10, fontStyle));

    fontStyle.fontFace = FONT_NARROW;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 2;
    disp_tripCompUnit.add(row14->addArea(220, 70, 7, fontStyle));
    //row14->addArea(220, 80, L100KM, fontStyle);

    // outside temp value
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_outsideTemp.add(row12->addArea(350, 100, 9, fontStyle));
    disp_outsideTemp.add(row13->addArea(350, 100, 9, fontStyle));
    disp_outsideTemp.add(row14->addArea(350, 100, 9, fontStyle));

    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.alignment = ALIGN_LEFT;
    row12->addArea(0, 36, DEG_C, fontStyle);

    // clock
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 2;
    CFontArea* disp_AMPM = row14->addArea(550, 30, 2, fontStyle);


    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_clockH.add(row12->addArea(580, 100, 6, fontStyle));
    disp_clockH.add(row13->addArea(580, 100, 6, fontStyle));
    disp_clockH.add(row14->addArea(580, 100, 6, fontStyle));

    // clock colon
    fontStyle.blinkMode = BLINK_ALL;
    row12->addArea(0, fontStyle, 1, _topDot);
    row14->addArea(0, fontStyle, 1, _botDot);


    fontStyle.blinkMode = BLINK_NONE;
    disp_clockM.add(row12->addArea(692, 100, 6, fontStyle));
    disp_clockM.add(row13->addArea(692, 100, 6, fontStyle));
    disp_clockM.add(row14->addArea(692, 100, 6, fontStyle));

    map->addEmptyRow(9);
    rowStyle.tracking = 0;
    fontStyle.tracking = 0;
    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_BLACK;
    fontStyle.fontFace = FONT_GRAPHICS;
    CFontRow* line1top = map->addRow(12, rowStyle);
    line1top->addArea(15, fontStyle, 1,  MG_RD25pxBL);
    line1top->addArea(753, fontStyle, 1, MG_RD25pxBR);


    map->addEmptyRow(16);
    CFontRow* line1bot = map->addRow(12, rowStyle);
    line1bot->addArea(15, fontStyle, 1, MG_RD25pxTL);

    fontStyle.fontColor = CL_RX300_FOREGROUND;
    disp_brightness.add(line1bot->addArea(_DISP_SCALE_LEFT - 2, fontStyle, 11, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

    fontStyle.fontColor = CL_BLACK;
    line1bot->addArea(753, fontStyle, 1, MG_RD25pxTR);

    // section 2


    CFontRow* row20b = map->addRow(18, rowStyle);
    CFontRow* row20c = map->addRow(4, rowStyle);
    map->addEmptyRow(12);

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

    CFontRow* row20 = map->addRow(4, rowStyle);
    CFontRow* row21 = map->addRow(18, rowStyle);
    CFontRow* row22 = map->addRow(8, rowStyle);
    CFontRow* row23 = map->addRow(18, rowStyle);
    CFontRow* row24 = map->addRow(11, rowStyle);
    CFontRow* row25 = map->addRow(18, rowStyle);
    CFontRow* row26 = map->addRow(7, rowStyle);

    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 3;
    row21->addArea(_DISP_TEMP_LEFT, 80, "TEMP", fontStyle);

    // inside temp value
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.tracking = 0;
    disp_insideTemp.add(row23->addArea(_DISP_TEMP_LEFT, 140, 15, fontStyle));
    disp_insideTemp.add(row24->addArea(_DISP_TEMP_LEFT, 140, 15, fontStyle));
    disp_insideTemp.add(row25->addArea(_DISP_TEMP_LEFT, 140, 15, fontStyle));

    // fan
    fontStyle.fontFace = FONT_GRAPHICS;
    disp_fan.add(row20->addArea(_DISP_FAN_LEFT, fontStyle, 3, MG_FAN12, MG_FAN11, MG_FAN13));
    disp_fan.add(row21->addArea(_DISP_FAN_LEFT, fontStyle, 3, MG_FAN22, MG_FAN21, MG_FAN23));
    disp_fan.add(row22->addArea(_DISP_FAN_LEFT, fontStyle, 3, MG_FAN32, MG_FAN31, MG_FAN33));

    // scale
    row24->addArea(_DISP_SCALE_LEFT - 8, fontStyle, 1, MG_SHV);
    row25->addArea(_DISP_SCALE_LEFT - 8, fontStyle, 1, MG_SHV);
    BYTE i;
    WORD left;
    for(i = 0, left = _DISP_SCALE_LEFT; i < 7; i++, left += 19)
    {
        switch(i)
        {
        case 0:
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_TOP12, MG_SC_TOP11));
            break;
        case 1:
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_TOP22, MG_SC_TOP21));
            break;
        case 2:
            disp_scale[i].add(row23->addArea(left, fontStyle, 2, MG_SC_TOP32, MG_SC_TOP31));
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 3:
            disp_scale[i].add(row23->addArea(left, fontStyle, 2, MG_SC_TOP42, MG_SC_TOP41));
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 4:
            disp_scale[i].add(row23->addArea(left, fontStyle, 2, MG_SC_TOP52, MG_SC_TOP51));
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 5:
            disp_scale[i].add(row22->addArea(left, fontStyle, 2, MG_SC_TOP62, MG_SC_TOP61));
            disp_scale[i].add(row23->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        case 6:
            disp_scale[i].add(row21->addArea(left, fontStyle, 2, MG_SC_TOP72, MG_SC_TOP71));
            disp_scale[i].add(row22->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            disp_scale[i].add(row23->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            disp_scale[i].add(row24->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
            break;
        }
        disp_scale[i].add(row25->addArea(left, fontStyle, 2, MG_SC_FILL2, MG_SC_FILL1));
    }

    // scale right border
    row21->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV);
    row22->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV);
    row23->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV);
    row24->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV);
    row25->addArea(_DISP_SCALE_LEFT + 132, fontStyle, 1, MG_SHV);

    // scale bottom border
    row26->addArea(_DISP_SCALE_LEFT - 8, fontStyle, 12, MG_SHB, MG_SHBL, MG_SHBR);

    // windshield
    disp_windShield.add(row21->addArea(_DISP_WSHIELD_LEFT, fontStyle, 2, MG_WSHIELD12, MG_WSHIELD11));
    disp_windShield.add(row21->addArea(_DISP_WSHIELD_LEFT + 24, fontStyle, 2, MG_WSHIELD14, MG_WSHIELD13));
    disp_windShield.add(row22->addArea(_DISP_WSHIELD_LEFT + 7, fontStyle, 3, MG_WSHIELD22, MG_WSHIELD21, MG_WSHIELD23));
    disp_windShield.add(row23->addArea(_DISP_WSHIELD_LEFT + 16, fontStyle, 2, MG_WSHIELD32, MG_WSHIELD31));

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



    // legs arrow
    row22->addArea(_DISP_LEGS_ARROW_LEFT, fontStyle, 2, MG_V_ARROW12, MG_V_ARROW11);
    row23->addArea(_DISP_LEGS_ARROW_LEFT, fontStyle, 2, MG_V_ARROW22, MG_V_ARROW21);
    row24->addArea(_DISP_LEGS_ARROW_LEFT + 4, fontStyle, 2, MG_V_ARROW32, MG_V_ARROW31);

    // head arrow
    disp_headArrow.add(row21->addArea(_DISP_HEAD_ARROW_LEFT, fontStyle, 3, MG_H_ARROW_BODY, MG_H_ARROW_BODY, MG_H_ARROW_RT));
    disp_headArrow.add(row22->addArea(_DISP_HEAD_ARROW_LEFT + 14, fontStyle, 1, MG_H_ARROW_RB));

    // head
    row21->addArea(_DISP_BODY_LEFT + 56, fontStyle, 2, MG_ROUND_D17_R, MG_ROUND_D17_L);

    // body
    row22->addArea(_DISP_BODY_LEFT + 50, fontStyle, 2, MG_BODY12, MG_BODY11);

    row23->addArea(_DISP_BODY_LEFT + 15, fontStyle, 2, MG_BODY22, MG_BODY21);
    row23->addArea(_DISP_BODY_LEFT + 39, fontStyle, 2, MG_BODY24, MG_BODY23);

    row24->addArea(_DISP_BODY_LEFT + 9, fontStyle, 2, MG_BODY32, MG_BODY31);
    row24->addArea(_DISP_BODY_LEFT + 31, fontStyle, 2, MG_BODY34, MG_BODY33);

    row25->addArea(_DISP_BODY_LEFT, fontStyle, 2, MG_BODY42, MG_BODY41);
    row25->addArea(_DISP_BODY_LEFT + 42, fontStyle, 1, MG_BODY43);

    // section 3
    map->addEmptyRow(58);
    CFontRow* row31 = map->addRow(18, rowStyle);
    CFontRow* row32 = map->addRow(11, rowStyle);
    CFontRow* row33 = map->addRow(18, rowStyle);
    map->addEmptyRow(34);
    CFontRow* row34 = map->addRow(7, rowStyle);
    CFontRow* row35 = map->addRow(8, rowStyle);
    CFontRow* row36 = map->addRow(18, rowStyle);
    CFontRow* row37 = map->addRow(6, rowStyle);
    CFontRow* row38 = map->addRow(12, rowStyle);

    // media text 1
    #define _DISP_MEDIA_TEXT_1_LEFT      70
    #define _DISP_MEDIA_TEXT_2_LEFT      320
    #define _DISP_MEDIA_TEXT_3_LEFT      480
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    disp_mediaText1.add(row31->addArea(_DISP_MEDIA_TEXT_1_LEFT, 140, 11, fontStyle));
    disp_mediaText1.add(row32->addArea(_DISP_MEDIA_TEXT_1_LEFT, 140, 11, fontStyle));
    disp_mediaText1.add(row33->addArea(_DISP_MEDIA_TEXT_1_LEFT, 140, 11, fontStyle));

    // disc
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.tracking = 2;
    row31->addArea(_DISP_MEDIA_TEXT_2_LEFT - 60, 50, "DISC", fontStyle);

    // channel label
    row33->addArea(_DISP_MEDIA_TEXT_2_LEFT - 60, 50, "CH", fontStyle);

    // media text 2
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 0;
    disp_mediaText2.add(row31->addArea(_DISP_MEDIA_TEXT_2_LEFT, 40, 3, fontStyle));
    disp_mediaText2.add(row32->addArea(_DISP_MEDIA_TEXT_2_LEFT, 40, 3, fontStyle));
    disp_mediaText2.add(row33->addArea(_DISP_MEDIA_TEXT_2_LEFT, 40, 3, fontStyle));

    // track
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 2;
    row31->addArea(_DISP_MEDIA_TEXT_3_LEFT - 90, 70, "TRACK", fontStyle);

    // mute
    row33->addArea(_DISP_MEDIA_TEXT_3_LEFT - 90, 70, "MUTE", fontStyle);

    // media text 3
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 0;
    disp_mediaText3.add(row31->addArea(_DISP_MEDIA_TEXT_3_LEFT, 224, 21, fontStyle));
    disp_mediaText3.add(row32->addArea(_DISP_MEDIA_TEXT_3_LEFT, 224, 21, fontStyle));
    disp_mediaText3.add(row33->addArea(_DISP_MEDIA_TEXT_3_LEFT, 224, 21, fontStyle));

    // ST
    fontStyle.fontFace = FONT_BASIC;
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.tracking = 2;
    row31->addArea(710, 30, "ST", fontStyle);

    // metal
    row33->addArea(710, 70, "METAL", fontStyle);

    // bottom left corner
    fontStyle.fontColor = CL_BLACK;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.tracking = 0;
    row38->addArea(15, fontStyle, 1,  MG_RD25pxBL);

    //fontStyle.bgColor = CL_BLACK;

    // buttons
    #define _DISP_BUTTON_WIDTH      164
    #define _DISP_BUTTON_DISTANCE   19
    #define _DISP_BUTTONS_LEFT      31
    fontStyle.fontColor = CL_RX300_FOREGROUND;

    for(i = 0; i < 4; i++)
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
        disp_button[i] = row36->addArea(0, _DISP_BUTTON_WIDTH / 2 - 2, "SCAN", fontStyle);
        disp_button[i + 1] = row36->addArea(0, _DISP_BUTTON_WIDTH / 2 - 2, "PSCAN", fontStyle);

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
    row38->addArea(753, fontStyle, 1, MG_RD25pxBR);
}
//-----------------------------------------------------------------------------
