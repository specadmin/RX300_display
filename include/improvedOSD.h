#ifndef IMPROVED_OSD_H
#define IMPROVED_OSD_H
//-----------------------------------------------------------------------------
#include "RTD2660AVR/OSD.h"
//-----------------------------------------------------------------------------
class CImprovedAreaSet : public CFontAreaSet
{
private:
    void print3l(const char* str);
public:
    void print(const char* str);
};
//-----------------------------------------------------------------------------
#endif
