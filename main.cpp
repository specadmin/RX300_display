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
    FONT_BASIC,
    FONT_GRAPHICS,
    FONT_CRYSTAL
};
//-----------------------------------------------------------------------------
#define DEG_C   "\014\015"
#define KMPH    "\017\020\021\022"
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
int main()
{
    DEBUG_INIT();
    //CImprovedOSD OSD;
    struct DisplayConfig config;
    config.backgroundColor = 0x000000;
    display.init(config);


//    display.showVideo(VS_AV2);
    display.on();


    OSD.setOrigin(0,0);
    OSD.uploadColorPallete((BYTE*)pallete);
    OSD.setTransparency(2, BLEND_ALL);

    struct FontRowStyle rowStyle;
    struct FontAreaStyle fontStyle;

    CFontMap* map1 = &OSD.maps[0];
    CFontMap* map2 = &OSD.maps[1];


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
    OSD.window[0] = createWindow(wndConf);

    wndConf.top = 134;
    wndConf.height = 16;
    wndConf.bodyColor = CL_BLACK;
    OSD.window[1] = createWindow(wndConf);

    wndConf.top = 296;
    wndConf.height = 6;
    OSD.window[2] = createWindow(wndConf);


    CFontAreaSet alarm;
    map1->addEmptyRow(11);
    CFontRow* line0top = map1->addRow(12, rowStyle);

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.fontColor = CL_BLACK;
    line0top->addArea(15, fontStyle, 1, MG_RD25pxTL);

    fontStyle.fontColor = CL_RX300_FOREGROUND;
    line0top->addArea(0, fontStyle, 23, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR);
    line0top->addArea(318, fontStyle, 14, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR);
    alarm.add(line0top->addArea(502, fontStyle, 21, MG_6PXSKIP, MG_RD8pxTL, MG_RD8pxTR));

    fontStyle.fontColor = CL_BLACK;
    line0top->addArea(753, fontStyle, 1, MG_RD25pxTR);


    fontStyle.alignment = ALIGN_CENTER;
    fontStyle.fontColor = CL_RX300_BACKGROUND;
    fontStyle.bgColor = CL_RX300_FOREGROUND;
    fontStyle.fontFace = FONT_BASIC;
    rowStyle.tracking = 2;
    CFontRow* titles1 = map1->addRow(18, rowStyle);

    CFontArea* cruise = titles1->addArea(27, 276, "CRUISE INFORMATION", fontStyle);
    titles1->addArea(318, 168, "OUTSIDE TEMP", fontStyle);

    alarm.add(titles1->addArea(502, 252, "ALARM", fontStyle));

    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_GRAPHICS;
    rowStyle.tracking = 0;

    CFontRow* line0bot = map1->addRow(4, rowStyle);

    line0bot->addArea(27, fontStyle, 23, MG_FILL, MG_RD8pxBL, MG_RD8pxBR);
    line0bot->addArea(318, fontStyle, 14, MG_FILL, MG_RD8pxBL, MG_RD8pxBR);
    alarm.add(line0bot->addArea(502, fontStyle, 21, MG_FILL, MG_RD8pxBL, MG_RD8pxBR));
    map1->addEmptyRow(22);
    fontStyle.alignment = ALIGN_LEFT;
    fontStyle.fontColor = CL_RX300_FOREGROUND;
    fontStyle.bgColor = CL_TRANSPARENT;
    fontStyle.fontFace = FONT_BASIC;
    rowStyle.tracking = 0;
    CFontRow* titles2 = map1->addRow(18, rowStyle);
    fontStyle.tracking = 2;
    titles2->addArea(27, 50, "AVG", fontStyle);


    CFontRow* titles3 = map1->addRow(11, rowStyle);
    CFontRow* titles4 = map1->addRow(18, rowStyle);

    CImprovedAreaSet tripComp;
    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.tracking = 0;
    tripComp.add(titles2->addArea(85, fontStyle, 13));
    tripComp.add(titles3->addArea(85, fontStyle, 13));
    tripComp.add(titles4->addArea(85, fontStyle, 13));

    fontStyle.fontFace = FONT_GRAPHICS;
    fontStyle.tracking = 2;
    //titles4->addArea(0, 50, KMPH, fontStyle);

    fontStyle.fontFace = FONT_CRYSTAL;
    fontStyle.tracking = 0;
    CImprovedAreaSet outsideTemp;
    outsideTemp.add(titles2->addArea(400, fontStyle, 6));
    outsideTemp.add(titles3->addArea(400, fontStyle, 6));
    outsideTemp.add(titles4->addArea(400, fontStyle, 6));

    fontStyle.fontFace = FONT_GRAPHICS;
    titles2->addArea(450, 36, DEG_C, fontStyle);

    fontStyle.fontFace = FONT_BASIC;

    fontStyle.alignment = ALIGN_RIGHT;
    fontStyle.tracking = 2;
    CFontArea* AMPM = titles4->addArea(550, fontStyle, 2);

    map1->addEmptyRow(2);
    rowStyle.tracking = 0;
    fontStyle.alignment = ALIGN_JUSTIFY;
    fontStyle.fontColor = CL_BLACK;
    fontStyle.fontFace = FONT_GRAPHICS;
    CFontRow* line1top = map1->addRow(12, rowStyle);
    line1top->addArea(15, fontStyle, 1,  MG_RD25pxBL);
    line1top->addArea(753, fontStyle, 1, MG_RD25pxBR);

    map1->addEmptyRow(16);

    CFontRow* line1bot = map1->addRow(12, rowStyle);
    line1bot->addArea(15, fontStyle, 1, MG_RD25pxTL);
    line1bot->addArea(753, fontStyle, 1, MG_RD25pxTR);

    //alarm.hide();
    //display.hideBackground();
    OSD.show(map1);
    //halt();
    AMPM->print("AM");
    tripComp.print("1289");
    outsideTemp.print("24");


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


    memory_usage();


    //display.showVideo(VS_AV2);
    //delay(4);
    //display.hideVideo();
    //OSD.setTransparency(3, BLEND_ALL);

    delay(2);




    return 0;
}
//-----------------------------------------------------------------------------
