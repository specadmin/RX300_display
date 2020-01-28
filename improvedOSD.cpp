#include "improvedOSD.h"
#include "avr-debug/debug.h"
//-----------------------------------------------------------------------------
void CImprovedAreaSet::print3l(const char* str)
{
    register BYTE len = strlen(str);
    register BYTE size = 3 * len;
    char* str1 = (char*) malloc(size + 1);
    char* str2 = (char*) malloc(size + 1);
    char* str3 = (char*) malloc(size + 1);
    for(BYTE i = 0, j = 0; i < len; i++, j+=3)
    {
        switch(str[i])
        {
        case '0':
            str1[j] = _FA;  str1[j + 1] = _AB;
            str2[j] = _EFg; str2[j + 1] = _BCg;
            str3[j] = _DE;  str3[j + 1] = _CD;
            break;
        case '1':
            str1[j] = _empty;  str1[j + 1] = _aB;
            str2[j] = _empty;  str2[j + 1] = _BCg;
            str3[j] = _empty;  str3[j + 1] = _Cd;
            break;
        case '2':
            str1[j] = _fA;  str1[j + 1] = _AB;
            str2[j] = _EfG; str2[j + 1] = _BcG;
            str3[j] = _DE;  str3[j + 1] = _cD;
            break;
        case '3':
            str1[j] = _fA;  str1[j + 1] = _AB;
            str2[j] = _efG; str2[j + 1] = _BCG;
            str3[j] = _De;  str3[j + 1] = _CD;
            break;
        case '4':
            str1[j] = _Fa;  str1[j + 1] = _aB;
            str2[j] = _eFG; str2[j + 1] = _BCG;
            str3[j] = _empty;  str3[j + 1] = _Cd;
            break;
        case '5':
            str1[j] = _FA;  str1[j + 1] = _Ab;
            str2[j] = _eFG; str2[j + 1] = _bCG;
            str3[j] = _De;  str3[j + 1] = _CD;
            break;
        case '6':
            str1[j] = _FA;  str1[j + 1] = _Ab;
            str2[j] = _EFG; str2[j + 1] = _bCG;
            str3[j] = _DE;  str3[j + 1] = _CD;
            break;
        case '7':
            str1[j] = _fA;     str1[j + 1] = _AB;
            str2[j] = _empty;  str2[j + 1] = _BCg;
            str3[j] = _empty;  str3[j + 1] = _Cd;
            break;
        case '8':
            str1[j] = _FA;  str1[j + 1] = _AB;
            str2[j] = _EFG; str2[j + 1] = _BCG;
            str3[j] = _DE;  str3[j + 1] = _CD;
            break;
        case '9':
            str1[j] = _FA;  str1[j + 1] = _AB;
            str2[j] = _eFG; str2[j + 1] = _BCG;
            str3[j] = _De;  str3[j + 1] = _CD;
            break;
        case 'C':
            str1[j] = _FA;  str1[j + 1] = _Ab;
            str2[j] = _EFg; str2[j + 1] = _empty;
            str3[j] = _DE;  str3[j + 1] = _cD;
            break;
        case 'E':
            str1[j] = _FA;  str1[j + 1] = _Ab;
            str2[j] = _EFG; str2[j + 1] = _bcG;
            str3[j] = _DE;  str3[j + 1] = _cD;
            break;
        case 'F':
            str1[j] = _FA;  str1[j + 1] = _Ab;
            str2[j] = _EFG; str2[j + 1] = _bcG;
            str3[j] = _dE;  str3[j + 1] = _empty;
            break;
        case 'S':
            str1[j] = _FA;  str1[j + 1] = _Ab;
            str2[j] = _eFG; str2[j + 1] = _bCG;
            str3[j] = _De;  str3[j + 1] = _CD;
            break;
        case ' ':
            str1[j] = _empty;   str1[j + 1] = _empty;
            str2[j] = _empty;   str2[j + 1] = _empty;
            str3[j] = _empty;   str3[j + 1] = _empty;
            break;
        case '-':
            str1[j] = _empty;   str1[j + 1] = _empty;
            str2[j] = _efG;     str2[j + 1] = _bcG;
            str3[j] = _empty;   str3[j + 1] = _empty;
            break;
        case '.':
            str1[j] = _empty_dot;
            str2[j] = _empty_dot;
            str3[j] = _dot;
            j--;
            size--;
            break;
        default:
            DSTR("Unknown character");
        }
        str1[j + 2] = _tracking;
        str2[j + 2] = _tracking;
        str3[j + 2] = _tracking;
    }
    str1[size] = '\0';
    str2[size] = '\0';
    str3[size] = '\0';
    areas[0]->print(str1);
    areas[1]->print(str2);
    areas[2]->print(str3);
    free(str1);
    free(str2);
    free(str3);
}
//-----------------------------------------------------------------------------
void CImprovedAreaSet::print(const char* str)
{
    switch(areasCount)
    {
    case 1:
        areas[0]->print(str);
        break;
    case 3:
        print3l(str);
        break;
    default:
        DSTR("Incorrect area set for printing");
    }
}
//-----------------------------------------------------------------------------
