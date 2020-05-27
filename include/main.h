#ifndef MAIN_H
#define MAIN_H
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
    MG_UP,
    MG_DOWN,
    MG_FWD,
    MG_BKWD,
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
    MG_SHV_2,
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
    MG_FUEL_1, MG_FUEL_2,
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
    MG_CAR_BACK_1, MG_CAR_BACK_2, MG_CAR_BACK_3,

    MG_CAR_RL_DOOR_OPEN_1,
    MG_CAR_L_DOORS_2_ALL_OPEN,
    MG_CAR_FL_DOOR_OPEN_1,

    MG_CAR_RR_DOOR_OPEN_1,
    MG_CAR_R_DOORS_2_ALL_OPEN,
    MG_CAR_FR_DOOR_OPEN_1,

    MG_CAR_L_DOORS_12,
    MG_CAR_L_DOORS_22,
    MG_CAR_L_DOORS_32,
    MG_CAR_L_FRONT_ROOF,
    MG_CAR_R_FRONT_ROOF,
    MG_CAR_R_DOORS_32,
    MG_CAR_R_DOORS_22,
    MG_CAR_R_DOORS_12,
    MG_CAR_L_SIDE_11, MG_CAR_L_SIDE_12,
    MG_CAR_R_SIDE_11, MG_CAR_R_SIDE_12,
    MG_CAR_L_SIDE_21,
    MG_CAR_FRONT_CENTER,
    MG_CAR_R_SIDE_21,

    MG_TIRE_LINE,
    MG_FL_TIRE_1, MG_FL_TIRE_2,
    MG_FR_TIRE_1, MG_FR_TIRE_2,
    MG_RL_TIRE,
    MG_RR_TIRE,
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
enum
{
    MSG_MPX_DATA = 1,
    MSG_IEBUS_DATA,
    MSG_SHOW_FAN_SPEED,
};

//-----------------------------------------------------------------------------
#define KM      "\004\005"
#define UP      "\006"
#define DOWN    "\007"
#define FWD2    "\010\010"
#define BKWD2   "\011\011"
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
#define _DISP_CLOCK_LEFT            (X_OFFSET + 525)
#define _DISP_TEMP_LEFT             (X_OFFSET + 205)
#define _DISP_FAN_LEFT              (X_OFFSET + 355)
#define _DISP_SCALE_LEFT            (X_OFFSET + 353)
#define _DISP_AUTO_LEFT             (X_OFFSET + 530)
#define _DISP_WSHIELD_LEFT          (X_OFFSET + 555)
#define _DISP_BODY_LEFT             (X_OFFSET + 627)
#define _DISP_CAR_LEFT              (X_OFFSET + 70)
#define _DISP_HEAD_ARROW_LEFT       (_DISP_BODY_LEFT + 14)
#define _DISP_LEGS_ARROW_LEFT       (_DISP_BODY_LEFT - 18)
#define _DISP_BUTTON_WIDTH          164
#define _DISP_BUTTON_DISTANCE       19
#define _DISP_BUTTONS_LEFT          (X_OFFSET + 6)
#define _DISP_MEDIA_TEXT_1_LEFT     (X_OFFSET + 15)
#define _DISP_MEDIA_TEXT_2_LEFT     (X_OFFSET + 255)
#define _DISP_MEDIA_TEXT_3_LEFT     (X_OFFSET + 401)
#define FRONT_LEFT                  0
#define FRONT_RIGHT                 1
#define REAR_LEFT                   2
#define REAR_RIGHT                  3

//-----------------------------------------------------------------------------

#endif
