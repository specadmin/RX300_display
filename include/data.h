#ifndef DATA_INCLUDED
#define DATA_INCLUDED
//-----------------------------------------------------------------------------
struct BYTE_DATA
{
    BYTE value;
    BYTE prevValue;
    BYTE updated;
    BYTE_DATA()
    {
        value = 0xFF;
        prevValue = 0xFF;
        updated = 0;
    }
    BYTE_DATA(BYTE new_value)
    {
        value = new_value;
    }
    void operator=(BYTE new_value)
    {
        value = new_value;
        updated = 1;
    }
    operator bool()
    {
        return (bool) value;
    }
};
//-----------------------------------------------------------------------------
struct WORD_DATA
{
    WORD value;
    WORD prevValue;
    BYTE updated;
    WORD_DATA()
    {
        value = 0xFFFF;
        prevValue = 0xFFFF;
        updated = 0;
    }
    WORD_DATA(WORD new_value)
    {
        value = new_value;
    }
    void operator=(WORD new_value)
    {
        value = new_value;
        updated = 1;
    }
};
//-----------------------------------------------------------------------------
#endif // DATA_H_INCLUDED
