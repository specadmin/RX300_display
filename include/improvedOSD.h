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

    _dot,

    _fA, _Fa, _FA,
    _aB, _Ab, _AB,

    _efG, _eFg, _Efg, _eFG, _EfG, _EFg, _EFG,
    _bcG, _bCg, _Bcg, _bCG, _BcG, _BCg, _BCG,

    _dE, _De, _DE,
    _cD, _Cd, _CD
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
