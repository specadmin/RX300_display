#ifndef OPTIONS_H
#define OPTIONS_H
//------------------------------------------------------------------------------
#include "units.h"
//------------------------------------------------------------------------------
struct SOptions
{
    bool showFPS = false;
    bool invertFuelRate = true;
    bool convertKm2Miles = false;
    bool convertLitres2GalonsUSA = false;
    bool convertLitres2GalonsENG = false;
    const char* distUnits = units.KM;;
    const char* speedUnits = units.KMPH;
    const char* fuelRateUnits = units.L100KM;
    const char* fuelRatePerTimeUnits = units.LPH;
};
//------------------------------------------------------------------------------
extern struct SOptions options;
//------------------------------------------------------------------------------
#endif
