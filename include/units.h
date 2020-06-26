#ifndef UNITS_H
#define UNITS_H
//------------------------------------------------------------------------------
struct SUnits
{
    const char KM[3] = "\004\005";
    const char MILES[2] = "\010";
    const char KMPH[5] = "\004\005\006\007";
    const char L100KM[8] = "\003\006\002\001\001\004\005";
    const char LPH[4] = "\003\006\007";
    const char GPH[4] = "\013\006\007";
    const char MPH[4] = "\010\011\012";
    const char MPG[4] = "\010\011\013";
    const char DEG_C[3] = "\003\004";
};
//------------------------------------------------------------------------------
extern SUnits units;
//------------------------------------------------------------------------------
#endif