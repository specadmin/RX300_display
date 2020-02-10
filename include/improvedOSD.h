#ifndef IMPROVED_OSD_H
#define IMPROVED_OSD_H
//-----------------------------------------------------------------------------
#include "RTD2660AVR/OSD.h"
//-----------------------------------------------------------------------------
enum
{
    _empty = 1,

    _tracking,

    _empty_dot,

    _topDot, _botDot,

    _dot, _dash,

    _fA, _Fa, _FA, _AI, _aFN,
    _aB, _Ab, _AB, _aBM,

    _efG, _eFg, _Efg, _eFG, _EfG, _EFg, _EFG, _IJ, _EFgN,
    _bcG, _bCg, _Bcg, _bCG, _BcG, _BCg, _BCG, _BCgM, _BCgO,

    _dE, _De, _DE, _DJ,
    _cD, _Cd, _CD, _CdO, _O,

    _K1, _K2,
    _KL1, _KL2, _KL2T,
    _L1, _L2, _L2T,

    _AK1, _AK2,
    _aKL1,
    _aL1,
};
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
