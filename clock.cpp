//-----------------------------------------------------------------------------
#include "avr-misc/avr-misc.h"
#include "avr-debug/debug.h"
#include "DS3231/ds3231.h"
#include "data.h"
#include "clock.h"
#include "body.h"
#include "tripComp.h"
#include "options.h"
//-----------------------------------------------------------------------------
static BYTE_DATA    hours;
static BYTE_DATA    minutes;
static BYTE_DATA    alarmRings;
static bool         clockSet = false;
static bool         clockVisible = false;
static char         str[3] = { 0, 0, 0 };
CImprovedAreaSet    disp_clockH;
CImprovedAreaSet    disp_clockM;
CFontAreaSet        disp_clockColon;
CFontAreaSet        disp_alarm;
extern BYTE FPS;
//-----------------------------------------------------------------------------
void time_changed(__unused BYTE result, BYTE datetime[7])
{
    hours = datetime[HOUR];
    minutes = datetime[MIN];
    tripDay = datetime[DAY];
}
//-----------------------------------------------------------------------------
__inline void clockHide()
{
    disp_clockH.hide();
    disp_clockM.hide();
    clockVisible = false;
}
//-----------------------------------------------------------------------------
__inline void clockEnable()
{
    RTC.enable_SQW_output(RTC_SQW_FREQ_1HZ);
    RTC.alarm2.set_every_minute();
    RTC.get_datetime_bcd(time_changed);
    // enabe pin-change interrupt
    set_bit(PCMSK0, PCINT0);
    set_bit(PCIFR, PCIF0);
    set_bit(PCICR, PCIE0);
}
//-----------------------------------------------------------------------------
void RTC_init()
{
    clockHide();
    if(RTC.connected())
    {
        RTC.alarm1.unset();
        RTC.alarm2.unset();
        RTC.enable_run_on_battery();
        RTC.disable_32kHz_output();
        RTC.disable_SQW_output();
        if(!RTC.time_lost())
        {
            clockSet = true;
            clockEnable();
        }
    }
}
//-----------------------------------------------------------------------------
void alarms_receiever(BYTE alarms)
{
    if(alarms & 0x02)
    {
        RTC.get_time_bcd(time_changed);
    }
    if(alarms & 0x01)
    {
        alarmRings = 1;
    }
    tripTimer++;
    if(selectorAT & 0x1F)
    {
        totalDriveTime++;
    }
    if(options.showFPS)
    {
        DVAR(FPS);
        FPS = 0;
    }
}
//-----------------------------------------------------------------------------
void displayClock()
{
    if(clockSet && !clockVisible)
    {
        disp_clockH.show();
        disp_clockM.show();
        disp_clockColon.blink();
        clockVisible = true;
    }
    if(hours.updated)
    {
        str[0] = (hours.value >> 4) + '0';
        str[1] = (hours.value & 0x0F) + '0';
        disp_clockH.print(str);
    }
    if(minutes.updated)
    {
        str[0] = (minutes.value >> 4) + '0';
        str[1] = (minutes.value & 0x0F) + '0';
        disp_clockM.print(str);
    }
}
//-----------------------------------------------------------------------------
ISR(PCINT0_vect)
{
    if(!test_bit(PINA, 0))
    {
        RTC.clear_alarm_events(&alarms_receiever);
    }
}
//-----------------------------------------------------------------------------
